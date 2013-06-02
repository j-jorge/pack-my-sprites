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
/**
 * \file
 * \brief Implementation of the sdc::gimp_interface class.
 * \author Julien Jorge
 */
#include "gimp_interface.hpp"

#include <sstream>
#include <cstdio>

#include <boost/filesystem/convenience.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param gimp_console_program The path to the gimp-console executable.
 * \param xcfinfo_program The path to the xcfinfo executable.
 * \param scheme_directory The paths to the directories where the scheme scripts
 *        are searched.
 */
sdc::gimp_interface::gimp_interface
( path_list_type scheme_directory, std::string gimp_console_program )
  : m_scheme_directory( scheme_directory ),
    m_gimp_console_program( gimp_console_program )
{

} // gimp_interface::gimp_interface()

/*----------------------------------------------------------------------------*/
/**
 * \brief Executes a given Scheme script using the GIMP.
 * \papam script The script to execute.
 * \param includes The files to include before executing the script.
 * \return The output of the script.
 */
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
} // gimp_interface::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Executes gimp-console on a given Scheme script.
 * \param script The script to pass to gimp.
 */
std::string
sdc::gimp_interface::execute_gimp_scheme_process( std::string script ) const
{
  const std::string command
    ( m_gimp_console_program + " --no-data --no-fonts --batch -" );
  FILE* process = popen( command.c_str(), "w" );

  if ( process == NULL )
    {
      std::cerr << "Failed to execute gimp console: '" << command << "'"
                << std::endl;
      return std::string();
    }

  fputs( script.c_str(), process );

  const std::size_t buffer_length( 512 );
  char buffer[ buffer_length ];
  std::ostringstream oss;

  while( fgets( buffer, buffer_length, process ) != NULL )
    oss << buffer;

  pclose( process );

  return oss.str();
} // gimp_interface::execute_gimp_scheme_process()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the full path of a Scheme file.
 *
 * The file is searched in m_scheme_directory and the first match is returned.
 *
 * \param filename The name of the file to search.
 * \return The path to the first file found with the paths of m_scheme_directory
 *         or filename if the file was not found.
 */
std::string sdc::gimp_interface::get_scheme_path( std::string filename ) const
{
  for ( path_list_type::const_iterator it=m_scheme_directory.begin();
        it!=m_scheme_directory.end();
        ++it )
    {
      boost::filesystem::path p( *it, boost::filesystem::native );
      p /= filename;

      if ( boost::filesystem::exists( p ) )
        return p.string();
    }

  return filename;
} // gimp_interface::get_scheme_path()
