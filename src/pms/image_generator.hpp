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
#ifndef __PMS_IMAGE_GENERATOR_HPP__
#define __PMS_IMAGE_GENERATOR_HPP__

#include "gimp_interface.hpp"
#include "sprite_sheet.hpp"
#include "working_directory.hpp"

#include <claw/image.hpp>

namespace pms
{
  class image_generator
  {
  public:
    explicit image_generator( const gimp_interface& gimp );

    void generate( const std::string& source, const sprite_sheet& sheet ) const;

  private:
    void generate_output_with_internal_tool
    ( const working_directory& dir, const image_map& images,
      const spritedesc& desc ) const;
    
    void copy_sprite
    ( claw::graphic::image& result, const std::string& file_path,
      const spritedesc::sprite& sprite ) const;

    void rotate( claw::graphic::image& image ) const;
    void bleed
    ( claw::graphic::image& result, const claw::graphic::image& image,
      const claw::math::coordinate_2d< int >& position ) const;

    void generate_output_with_gimp
    ( const working_directory& dir, const image_map& images,
      const spritedesc& desc ) const;

    void generate_scm
    ( std::ostream& os, const working_directory& dir, const image_map& images,
      const spritedesc& desc ) const;

    void generate_scm
    ( std::ostream& os, const image_info& images, const spritedesc::sprite& s,
      const std::string& target_id ) const;

    std::string make_image_varname( const std::string& id ) const;

  private:
    gimp_interface m_gimp;

  };
}

#endif
