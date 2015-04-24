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
#ifndef __PMS_WORKING_DIRECTORY_HPP__
#define __PMS_WORKING_DIRECTORY_HPP__

#include <string>

namespace pms
{
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
    const std::string m_input_file_name;
    std::string m_input_directory;
    std::string m_output_directory;

  };

}

#endif
