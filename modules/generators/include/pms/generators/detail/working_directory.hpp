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

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      class working_directory
      {
      public:
        explicit working_directory( const std::string& input_file_name );

        const std::string& get_input_file_name() const;
        std::string get_image_path( const std::string& image_name ) const;

        std::string get_output_file_path
        ( const std::string& sheet_name, const std::string& extension ) const;
        std::string get_output_file_path
        ( const std::string& sheet_name, std::size_t index, std::size_t range,
          const std::string& extension ) const;
        std::string get_output_file_name
        ( const std::string& sheet_name, std::size_t index, std::size_t range,
          const std::string& extension ) const;

      private:
        const std::string m_input_file_name;
        std::string m_directory;
      };
    }
  }
}
