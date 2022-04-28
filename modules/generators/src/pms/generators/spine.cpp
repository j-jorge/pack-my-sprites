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
#include "pms/generators/spine.hpp"

#include "pms/generators/detail/working_directory.hpp"

#include <claw/logger/logger.hpp>

#include <fstream>

void pms::generators::spine::generate
( const std::string& spritedesc_file_path,
  const layout::atlas& atlas ) const
{
  const detail::working_directory dir( spritedesc_file_path );

  claw::logger << claw::log_verbose
               << "Generating spine file for sprite sheet '"
               << atlas.output_name << "' of '"
               << spritedesc_file_path << "'"
               << std::endl;

  generate_file( dir, atlas );
}

void pms::generators::spine::generate_file
( const detail::working_directory& dir, const layout::atlas& atlas ) const
{
  const std::string filename
    ( dir.get_output_file_path( atlas.output_name, "atlas" ) );

  std::ofstream f( filename );
  const std::size_t page_count( atlas.pages.size() );

  for ( std::size_t i( 0 ); i != page_count; ++i )
    f << get_page
      ( dir.get_output_file_name( atlas.output_name, i, page_count, "" ),
        atlas.pages[ i ] );
}

std::string
pms::generators::spine::get_page
( const std::string& base_name, const layout::atlas_page& desc ) const
{
  std::string result( get_page_header( base_name, desc ) );

  for ( auto it( desc.sprite_begin() ); it != desc.sprite_end(); ++it )
    result += get_sprite_entry( *it );

  return result;
}

std::string
pms::generators::spine::get_page_header
( const std::string& base_name, const layout::atlas_page& desc ) const
{
  std::ostringstream result;

  result << '\n'
         << base_name << ".png\n"
         << "size: " << desc.width << ',' << desc.height << '\n'
         << "format: RGBA8888\n"
         << "filter: Linear,Linear\n"
         << "repeat: none\n";

  return result.str();
}

std::string
pms::generators::spine::get_sprite_entry
( const layout::atlas_page::sprite& sprite ) const
{
  const int x( sprite.result_box.position.x );
  const int y( sprite.result_box.position.y );
  const int width( sprite.result_box.width );
  const int height( sprite.result_box.height );

  std::ostringstream result;

  result << sprite.name << '\n'
         << "  rotate: " << ( sprite.rotated ? "true" : "false" ) << '\n'
         << "  xy: " << x << ", " << y << '\n'
         << "  size: " << width << ", " << height << '\n'
         << "  orig: " << sprite.display_offsets.width
         << ", " << sprite.display_offsets.height << '\n'
         << "  offset: " << sprite.display_offsets.position.x << ", "
         << sprite.display_offsets.position.y << "\n"
         << "  index: -1\n";

  return result.str();
}
