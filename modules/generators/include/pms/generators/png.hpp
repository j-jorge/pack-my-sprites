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

#include "pms/gimp/system_interface.hpp"
#include "pms/layout/atlas.hpp"

#include <claw/image.hpp>

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      class working_directory;
    }

    enum class color_mode;
    enum class rotation_direction;
    
    class png
    {
    public:
      png
      ( const gimp::system_interface& gimp, rotation_direction rotation,
        color_mode color );

      void generate
      ( const std::string& source, const layout::atlas& atlas ) const;
      
    private:
      void generate_output_with_internal_tool
      ( const detail::working_directory& dir,
        const layout::atlas& atlas ) const;
      void generate_output_with_internal_tool
      ( const detail::working_directory& dir, std::size_t index,
        const layout::atlas& atlas ) const;
    
      void copy_sprite
      ( claw::graphic::image& result, const std::string& file_path,
        const layout::atlas_page::sprite& sprite ) const;

      void rotate( claw::graphic::image& image ) const;
      void multiply_alpha( claw::graphic::image& image ) const;
      void bleed
      ( claw::graphic::image& result, const claw::graphic::image& image,
        const claw::math::coordinate_2d< int >& position ) const;

      void generate_output_with_gimp
      ( const detail::working_directory& dir,
        const layout::atlas& atlas ) const;
      void generate_output_with_gimp
      ( const detail::working_directory& dir, std::size_t index,
        const layout::atlas& atlas ) const;

      void generate_scm
      ( std::ostream& os, const detail::working_directory& dir,
        std::size_t index, const layout::atlas& atlas ) const;
      
      void generate_scm
      ( std::ostream& os, const resources::image& image,
        const layout::atlas_page::sprite& s,
        const std::string& target_id ) const;
      
      std::string make_image_varname( const std::string& id ) const;

    private:
      const rotation_direction m_rotation_direction;
      const color_mode m_color_mode;
      gimp::system_interface m_gimp;
    };
  }
}
