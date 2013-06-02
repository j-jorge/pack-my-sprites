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
 * \brief This class provides an interface to execute GIMP to process a given
 *        Scheme script.
 * \author Julien Jorge
 */
#ifndef __SDC_GIMP_INTERFACE_HPP__
#define __SDC_GIMP_INTERFACE_HPP__

#include "spritedesc_collection.hpp"
#include "working_directory.hpp"

namespace sdc
{
  /**
   * \brief This class provides an interface to execute GIMP to process a given
   *        Scheme script.
   * \author Julien Jorge
   */
  class gimp_interface
  {
  public:
    typedef std::list<std::string> path_list_type;

  public:
    gimp_interface
    ( path_list_type scheme_directory, std::string gimp_console_program );

    std::string run( std::string script, path_list_type includes ) const;

  private:
    std::string execute_gimp_scheme_process( std::string script ) const;

    std::string get_scheme_path( std::string filename ) const;

  private:
    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console_program;

  }; // class gimp_interface
} // namespace sdc

#endif // __SDC_GIMP_INTERFACE_HPP__
