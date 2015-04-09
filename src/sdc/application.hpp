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
 * \brief The main class.
 * \author Julien Jorge
 */
#ifndef __SDC_APPLICATION_HPP__
#define __SDC_APPLICATION_HPP__

#include "spritedesc_collection.hpp"

#include <claw/application.hpp>

#include <vector>

namespace sdc
{
  /**
   * \brief The main class.
   * \author Julien Jorge
   */
  class application:
    public claw::application
  {
  private:
    typedef std::map<std::string, spritedesc_collection> file_to_spritedesc_map;

    typedef std::list<std::string> path_list_type;

  public:
    application( int& argc, char** &argv );

    int run();

  private:
    void help() const;
    void check_arguments( int& argc, char** &argv );

    void process_files();
    spritedesc_collection read_spritedesc_stdin() const;
    spritedesc_collection read_spritedesc_file( std::string file_name ) const;
    spritedesc_collection read_spritedesc_file
    ( std::string directory, std::istream& in ) const;

    void generate_sprite_sheet_files
    ( file_to_spritedesc_map sprite_sheet_description ) const;
    void generate_sprite_sheet_files
    ( std::string source_file_path, spritedesc_collection desc ) const;

    std::string get_self_command() const;

  private:
    /** \brief Tells if we should quit immediatly. */
    bool m_quit;

    /** \brief The sprite description file. */
    std::vector<std::string> m_input_file;

    /** \brief The images to generate from the input files. */
    std::list<std::string> m_target;

    /** \brief Tells to generate the spritepos file. */
    bool m_generate_spritepos;

    /** \brief Tells to generate the PList file. */
    bool m_generate_plist;

    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console_program;

    /** \brief The name of the makefile to generate, if any. */
    std::string m_makefile;

  }; // class application
} // namespace sdc

#endif // __SDC_APPLICATION_HPP__
