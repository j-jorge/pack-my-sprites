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
#pragma once

#include "pms/gimp/system_interface.hpp"
#include "pms/layout/sprite_sheet.hpp"

#include <claw/image.hpp>

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      class working_directory;
    }
    
    class png
    {
    public:
      explicit png( const gimp::system_interface& gimp );

      void generate
      ( const std::string& source, const layout::sprite_sheet& sheet ) const;
      
    private:
      void generate_output_with_internal_tool
      ( const detail::working_directory& dir,
        const resources::image_mapping& images,
        const layout::description& desc ) const;
    
      void copy_sprite
      ( claw::graphic::image& result, const std::string& file_path,
        const layout::description::sprite& sprite ) const;

      void rotate( claw::graphic::image& image ) const;
      void bleed
      ( claw::graphic::image& result, const claw::graphic::image& image,
        const claw::math::coordinate_2d< int >& position ) const;

      void generate_output_with_gimp
      ( const detail::working_directory& dir,
        const resources::image_mapping& images,
        const layout::description& desc ) const;

      void generate_scm
      ( std::ostream& os, const detail::working_directory& dir,
        const resources::image_mapping& images,
        const layout::description& desc ) const;
      
      void generate_scm
      ( std::ostream& os, const resources::image& image,
        const layout::description::sprite& s,
        const std::string& target_id ) const;
      
      std::string make_image_varname( const std::string& id ) const;

    private:
      gimp::system_interface m_gimp;
    };
  }
}
