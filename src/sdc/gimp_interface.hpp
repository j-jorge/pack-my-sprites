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
#ifndef __SDC_GIMP_INTERFACE_HPP__
#define __SDC_GIMP_INTERFACE_HPP__

#include <string>
#include <list>

namespace sdc
{
  class gimp_interface
  {
  public:
    typedef std::list<std::string> path_list_type;

  public:
    gimp_interface();
    gimp_interface
    ( path_list_type scheme_directory, std::string gimp_console_program );

    std::string run( std::string script, path_list_type includes ) const;

  private:
    std::string execute_gimp_scheme_process( std::string script ) const;
    pid_t open_gimp_process( int& in_fd, int& out_fd ) const;

    std::string get_scheme_path( std::string filename ) const;

  private:
    path_list_type m_scheme_directory;
    std::string m_gimp_console_program;

  };
}

#endif
