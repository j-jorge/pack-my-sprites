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

#include "pms/layout/sprite_sheet.hpp"

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      class working_directory;
    }
    
    class css
    {
    public:
      void generate
      ( const std::string& spritedesc_file_path,
        const layout::sprite_sheet& sheet ) const;

    private:
      void generate_css
      ( const detail::working_directory& dir,
        const layout::sprite_sheet& sheet ) const;
      void generate_css
      ( std::ostream& os, const layout::description& desc,
        const std::string& sprite_sheet_name ) const;
      void generate_css_class
      ( std::ostream& os, const layout::description::sprite& sprite,
        const std::string& image_path ) const;
      void generate_css_class_rotated
      ( std::ostream& os, const layout::description::sprite& sprite,
        const std::string& image_path ) const;
      void generate_css_class_normal
      ( std::ostream& os, const layout::description::sprite& sprite,
        const std::string& image_path ) const;
    };
  }
}
