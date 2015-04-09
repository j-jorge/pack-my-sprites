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
 * \brief The paths to the directories where the files are searched and
 *        generated.
 * \author Julien Jorge
 */
#ifndef __SDC_WORKING_DIRECTORY_HPP__
#define __SDC_WORKING_DIRECTORY_HPP__

#include <string>

namespace sdc
{
  /**
   * \brief The paths to the directories where the files are searched and
   *        generated.
   * \author Julien Jorge
   */
  class working_directory
  {
  public:
    static bool is_fully_qualified( std::string filename );

    explicit working_directory( std::string input_file_name );

    std::string get_input_file_name() const;
    std::string get_xcf_path( std::string xcf_name ) const;
    std::string get_output_image_path( std::string sheet_name ) const;
    std::string
    get_output_file_path( std::string sheet_name, std::string extension ) const;

  private:
    /** \brief The file on which we work. */
    const std::string m_input_file_name;

    /** The directory where the files are searched. */
    std::string m_input_directory;

    /** The directory where the files are written. */
    std::string m_output_directory;

  }; // working_directory()

} // namespace sdc

#endif // __SDC_WORKING_DIRECTORY_HPP__
