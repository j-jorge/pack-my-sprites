/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
  
  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>
#include <vector>

#include <sys/types.h>

namespace pms
{
  namespace gimp
  {
    class system_interface
    {
    public:
      typedef std::vector<std::string> path_list_type;

    public:
      system_interface();
      system_interface
      ( const path_list_type& scheme_directory,
        const std::string& gimp_console_program );

      std::string run
      ( const std::string& script, const path_list_type& includes ) const;

    private:
      std::string
      execute_gimp_scheme_process( const std::string& script ) const;
      pid_t open_gimp_process( int& in_fd, int& out_fd ) const;

      std::string get_scheme_path( const std::string& filename ) const;

    private:
      path_list_type m_scheme_directory;
      std::string m_gimp_console_program;
    };
  }
}
