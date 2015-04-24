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
#ifndef __SDC_APPLICATION_HPP__
#define __SDC_APPLICATION_HPP__

#include "sprite_sheet.hpp"

#include <claw/application.hpp>

#include <vector>

namespace sdc
{
  class application:
    public claw::application
  {
  private:
    typedef std::map<std::string, sprite_sheet> file_to_spritedesc_map;

    typedef std::list<std::string> path_list_type;

  public:
    application( int& argc, char** &argv );

    int run();

  private:
    void help() const;
    void check_arguments( int& argc, char** &argv );

    void process_files();
    sprite_sheet read_spritedesc_stdin() const;
    sprite_sheet read_spritedesc_file( std::string file_name ) const;
    sprite_sheet read_spritedesc_file
    ( std::string directory, std::istream& in ) const;

    void generate_sprite_sheet_files
    ( file_to_spritedesc_map sprite_sheet_description ) const;
    void generate_sprite_sheet_files
    ( std::string source_file_path, sprite_sheet desc ) const;

    std::string get_self_command() const;

  private:
    bool m_quit;
    std::vector<std::string> m_input_file;
    bool m_generate_spritepos;
    bool m_generate_plist;
    path_list_type m_scheme_directory;
    std::string m_gimp_console_program;

  };
}

#endif
