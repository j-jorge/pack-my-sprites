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
#ifndef __PMS_CSS_GENERATOR_HPP__
#define __PMS_CSS_GENERATOR_HPP__

#include "sprite_sheet.hpp"
#include "working_directory.hpp"

namespace pms
{
  class css_generator
  {
  public:
    void generate
    ( const std::string& spritedesc_file_path,
      const sprite_sheet& sheet ) const;

  private:
    void generate_css
    ( const working_directory& dir, const spritedesc& desc ) const;
    void generate_css
    ( std::ostream& os, const spritedesc& desc,
      const std::string& sprite_sheet_name ) const;
    void generate_css_class
    ( std::ostream& os, const spritedesc::sprite& sprite,
      const std::string& image_path ) const;
    void generate_css_class_rotated
    ( std::ostream& os, const spritedesc::sprite& sprite,
      const std::string& image_path ) const;
    void generate_css_class_normal
    ( std::ostream& os, const spritedesc::sprite& sprite,
      const std::string& image_path ) const;

  };
}

#endif
