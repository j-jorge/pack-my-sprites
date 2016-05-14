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
#include "pms/generators/css.hpp"

#include "pms/generators/detail/working_directory.hpp"

#include <fstream>

#include <claw/logger.hpp>

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
  const layout::sprite_sheet& sheet ) const
{
  const detail::working_directory dir( spritedesc_file_path );

  claw::logger << claw::log_verbose
               << "Generating CSS file for sprite sheet '"
               << sheet.description.output_name << "' of '"
               << spritedesc_file_path << "'"
               << std::endl;

  generate_css( dir, sheet.description );
}

void pms::generators::css::generate_css
( const detail::working_directory& dir, const layout::description& desc ) const
{
  const std::string filename
    ( dir.get_output_file_path( desc.output_name, "css" ) );

  std::ofstream f( filename.c_str() );
  generate_css( f, desc, dir.get_output_file_path( desc.output_name, "png" ) );
}

void pms::generators::css::generate_css
( std::ostream& os, const layout::description& desc,
  const std::string& sprite_sheet_name ) const
{
  for ( auto it( desc.sprite_begin() ); it != desc.sprite_end(); ++it )
    {
      generate_css_class( os, *it, sprite_sheet_name );
      os << '\n';
    }
}

void pms::generators::css::generate_css_class
( std::ostream& os, const layout::description::sprite& sprite,
  const std::string& image_path ) const
{
  if ( sprite.rotated )
    generate_css_class_rotated( os, sprite, image_path );
  else
    generate_css_class_normal( os, sprite, image_path );
}

void pms::generators::css::generate_css_class_rotated
( std::ostream& os, const layout::description::sprite& sprite,
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
( std::ostream& os, const layout::description::sprite& sprite,
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
