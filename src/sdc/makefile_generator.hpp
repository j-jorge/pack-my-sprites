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
#ifndef __SDC_MAKEFILE_GENERATOR_HPP__
#define __SDC_MAKEFILE_GENERATOR_HPP__

#include "spritedesc_collection.hpp"
#include "working_directory.hpp"

#include <iostream>
#include <vector>

namespace sdc
{
  class makefile_generator
  {
  public:
    typedef std::map<std::string, spritedesc_collection> file_to_spritedesc_map;

    typedef std::list<std::string> path_list_type;

  public:
    makefile_generator( std::string makefile, std::string command );

    void run( file_to_spritedesc_map files );

  private:
    std::vector<std::string>
    get_all_output_files( file_to_spritedesc_map file ) const;

    std::vector<std::string>
    get_all_output_files
    ( working_directory dir, spritedesc_collection desc ) const;

    void generate_makefile_rules
    ( std::ostream& output, file_to_spritedesc_map file ) const;

    void generate_makefile_rule
    ( std::ostream& output, working_directory dir,
      std::list<spritedesc> desc ) const;

    void generate_makefile_rule
    ( std::ostream& output, working_directory dir, spritedesc desc ) const;

  private:
    std::string m_makefile;
    std::string m_command;

  };
}

#endif
