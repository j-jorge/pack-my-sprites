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
#include "pms/generators/css.hpp"

#include "pms/generators/detail/working_directory.hpp"

#include <claw/logger/logger.hpp>

#include <fstream>

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      static std::string make_class_name( const std::string& sprite_name );
    }
  }
}

void pms::generators::css::generate
( const std::string& spritedesc_file_path,
  const layout::atlas& atlas ) const
{
  const detail::working_directory dir( spritedesc_file_path );

  claw::logger << claw::log_verbose
               << "Generating CSS file for sprite sheet '"
               << atlas.output_name << "' of '"
               << spritedesc_file_path << "'"
               << std::endl;

  generate_css( dir, atlas );
}

void pms::generators::css::generate_css
( const detail::working_directory& dir,
  const layout::atlas& atlas ) const
{
  const std::string filename
    ( dir.get_output_file_path( atlas.output_name, "css" ) );

  std::ofstream f( filename.c_str() );

  const std::size_t page_count( atlas.pages.size() );

  for ( std::size_t i( 0 ); i != page_count; ++i )
    generate_css
      ( f, atlas.pages[ i ],
        dir.get_output_file_path( atlas.output_name, i, page_count, "png" ) );
}

void pms::generators::css::generate_css
( std::ostream& os, const layout::atlas_page& desc,
  const std::string& atlas_name ) const
{
  for ( auto it( desc.sprite_begin() ); it != desc.sprite_end(); ++it )
    {
      generate_css_class( os, *it, atlas_name );
      os << '\n';
    }
}

void pms::generators::css::generate_css_class
( std::ostream& os, const layout::atlas_page::sprite& sprite,
  const std::string& image_path ) const
{
  if ( sprite.rotated )
    generate_css_class_rotated( os, sprite, image_path );
  else
    generate_css_class_normal( os, sprite, image_path );
}

void pms::generators::css::generate_css_class_rotated
( std::ostream& os, const layout::atlas_page::sprite& sprite,
  const std::string& image_path ) const
{
  const int x( sprite.result_box.position.x );
  const int y( sprite.result_box.position.y );
  const int width( sprite.result_box.width );
  const int height( sprite.result_box.height );

  const std::string class_name( detail::make_class_name( sprite.name ) );

  os << "." << class_name << "::before {\n"
     << "  content: \"\";\n"
     << "  position: absolute;\n"
     << "  width: " << height << "px;\n"
     << "  height: " << width << "px;\n"
     << "  top: " << height << "px;\n"
     << "  left: 0;\n"
     << "  z-index: -1;\n"
     << "  background-image: url('" << image_path << "');\n"
     << "  background-position: -" << x << "px -" << y << "px;\n"
     << "  transform: rotate(-90deg);\n"
     << "  transform-origin: 0 0;\n"
     << "}\n\n"
     << "." << class_name << " {\n"
     << "  position: relative;\n"
     << "  width: " << width << "px;\n"
     << "  height: " << height << "px;\n"
     << "  z-index: 1;\n"
     << "}\n";
}

void pms::generators::css::generate_css_class_normal
( std::ostream& os, const layout::atlas_page::sprite& sprite,
  const std::string& image_path ) const
{
  const int x( sprite.result_box.position.x );
  const int y( sprite.result_box.position.y );
  const int width( sprite.result_box.width );
  const int height( sprite.result_box.height );

  os << "." << detail::make_class_name( sprite.name ) << " {\n"
     << "  background-image: url('" << image_path << "');\n"
     << "  background-position: -" << x << "px -" << y << "px;\n"
     << "  width: " << width << "px;\n"
     << "  height: " << height << "px;\n"
     << "}\n";
}

std::string pms::generators::detail::make_class_name
( const std::string& sprite_name )
{
  const std::string::size_type p( sprite_name.find_first_of( '.' ) );

  if ( p == std::string::npos )
    return sprite_name;
  else
    return sprite_name.substr( 0, p );
}
