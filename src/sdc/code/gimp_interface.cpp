/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Pack My Sprites.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gimp_interface.hpp"

#include <sstream>
#include <iostream>
#include <cstdio>
#include <csignal>

#include <boost/filesystem/convenience.hpp>

sdc::gimp_interface::gimp_interface()
{

}

sdc::gimp_interface::gimp_interface
( path_list_type scheme_directory, std::string gimp_console_program )
  : m_scheme_directory( scheme_directory ),
    m_gimp_console_program( gimp_console_program )
{

}

std::string sdc::gimp_interface::run
( std::string script, path_list_type includes ) const
{
  std::string full_script;

  for ( path_list_type::const_iterator it = includes.begin();
        it != includes.end(); ++it )
    full_script += "(load \"" + get_scheme_path( *it ) + "\")\n";

  full_script += script;
  full_script += "\n(gimp-quit 1)\n";

  return execute_gimp_scheme_process( full_script );
}

std::string
sdc::gimp_interface::execute_gimp_scheme_process( std::string script ) const
{
  int in_fd, out_fd;

  const int pid = open_gimp_process( in_fd, out_fd );

  if ( pid == -1 )
    return std::string();

  const char* script_data( script.c_str() );
  std::size_t remaining( script.size() );
  ssize_t written;

  do
    {
      written = write( in_fd, script_data, remaining );

      if ( written > 0 )
        {
          script_data += written;
          remaining -= written;
        }
    }
  while ( (remaining != 0) && (written > 0) );

  if ( close( in_fd ) != 0 )
    std::cerr << "Error " << errno << " while closing GIMP's stdin."
              << std::endl;

  if ( remaining != 0 )
    std::cerr << "The script could not be fully passed to GIMP." << std::endl;

  const std::size_t buffer_length( 512 );
  char buffer[ buffer_length ];
  std::ostringstream oss;
  ssize_t read_count;

  while( ( read_count = read( out_fd, buffer, buffer_length ) ) > 0 )
    oss << std::string(buffer, read_count);

  kill( pid, SIGTERM );
  close( out_fd );

  return oss.str();
}

pid_t
sdc::gimp_interface::open_gimp_process( int& in_fd, int& out_fd ) const
{
  int pipe_in[2];
  int pipe_out[2];

  if ( ( pipe( pipe_in ) != 0 ) || ( pipe( pipe_out ) != 0 ) )
    {
      std::cerr << "Call to pipe() has failed." << std::endl;
      return -1;
    }

  const pid_t pid = fork();

  if ( pid == -1 )
    {
      std::cerr << "Failed to fork process." << std::endl;
      return -1;
    }

  const int pipe_read(0);
  const int pipe_write(1);

  if ( pid == 0 )
    {
      // This is executed in the child process.
      close( pipe_in[ pipe_write ] );
      dup2( pipe_in[ pipe_read ], STDIN_FILENO );
      close( pipe_out[ pipe_read ] );
      dup2( pipe_out[ pipe_write ], STDOUT_FILENO );
      
      const int e =
        execlp
        ( m_gimp_console_program.c_str(), m_gimp_console_program.c_str(),
          "--no-data", "--no-fonts", "--batch", "-", (const char*)NULL );
      
      std::cerr << "Error " << errno
                << " (" << strerror( errno ) << ") when calling execl."
                << std::endl;

      _Exit(1);
    }

  close( pipe_in[ pipe_read ] );
  close( pipe_out[ pipe_write ] );

  in_fd = pipe_in[ pipe_write ];
  out_fd = pipe_out[ pipe_read ];

  return pid;
}

std::string sdc::gimp_interface::get_scheme_path( std::string filename ) const
{
  for ( path_list_type::const_iterator it=m_scheme_directory.begin();
        it!=m_scheme_directory.end();
        ++it )
    {
      boost::filesystem::path p( *it );
      p /= filename;

      if ( boost::filesystem::exists( p ) )
        return p.string();
    }

  return filename;
}
