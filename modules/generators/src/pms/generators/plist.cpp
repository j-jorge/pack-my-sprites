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
#include "pms/generators/plist.hpp"

#include "pms/generators/color_mode.hpp"
#include "pms/generators/detail/working_directory.hpp"

#include <fstream>

#include <claw/logger.hpp>

pms::generators::plist::plist( color_mode color )
  : m_color_mode( color )
{

}

void pms::generators::plist::generate
( const std::string& spritedesc_file_path,
  const layout::atlas& atlas ) const
{
  const detail::working_directory dir( spritedesc_file_path );

  claw::logger << claw::log_verbose
               << "Generating plist file for sprite sheet '"
               << atlas.output_name << "' of '"
               << spritedesc_file_path << "'"
               << std::endl;

  const std::size_t page_count( atlas.pages.size() );

  for ( std::size_t i( 0 ); i != page_count; ++i )
    {
      claw::logger << claw::log_verbose
                   << "Page " << ( i + 1 ) << '/' << page_count << "…\n";

      generate_plist( dir, i, atlas );
    }
}

void pms::generators::plist::generate_plist
( const detail::working_directory& dir, std::size_t index,
  const layout::atlas& atlas ) const
{
  const std::size_t page_count( atlas.pages.size() );
  const std::string filename
    ( dir.get_output_file_path
      ( atlas.output_name, index, page_count, "plist" ) );

  std::ofstream f( filename.c_str() );
  generate_plist
    ( f, dir.get_output_file_name( atlas.output_name, index, page_count, "" ),
      atlas.pages[ index ] );
}

void pms::generators::plist::generate_plist
( std::ostream& os, const std::string& base_name,
  const layout::atlas_page& desc ) const
{
  os << get_plist_header()
     << get_plist_body( base_name, desc );
}

std::string pms::generators::plist::get_plist_header() const
{
  return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\""
    " \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
}

std::string
pms::generators::plist::get_plist_body
( const std::string& base_name, const layout::atlas_page& desc ) const
{
  return std::string( "<plist version=\"1.0\">\n" )
    + "<dict>\n"
    + get_frames_entry( desc )
    + get_metadata_entry( base_name, desc )
    + "</dict>\n"
    + "</plist>\n";
}

std::string
pms::generators::plist::get_frames_entry
( const layout::atlas_page& desc ) const
{
  std::string result( "<key>frames</key>\n" );

  result += "<dict>\n";

  for ( auto it( desc.sprite_begin() ); it != desc.sprite_end(); ++it )
    result += get_sprite_entry( *it );

  result += "</dict>\n";

  return result;
}

std::string
pms::generators::plist::get_sprite_entry
( const layout::atlas_page::sprite& sprite ) const
{
  const int x( sprite.result_box.position.x );
  const int y( sprite.result_box.position.y );
  const int cropped_width( sprite.result_box.width );
  const int cropped_height( sprite.result_box.height );
  const int original_width( sprite.display_offsets.width );
  const int original_height( sprite.display_offsets.height );
  const int crop_offset_x( sprite.display_offsets.position.x );
  const int crop_offset_y( sprite.display_offsets.position.y );

  const int center_offset_x
    ( ( crop_offset_x + cropped_width / 2 ) - original_width / 2 );
  const int center_offset_y
    ( ( crop_offset_y + cropped_height / 2 ) - original_height / 2 );
  
  std::ostringstream result;

  result << "<key>" << sprite.name << "</key>\n"
    "<dict>\n"
    "<key>aliases</key>\n"
    "<array/>\n"
    "<key>spriteOffset</key>\n"
    "<string>{" << center_offset_x << ", "
         << -center_offset_y << "}</string>\n"
    "<key>spriteSize</key>\n"
    "<string>{" << cropped_width << ", " << cropped_height << "}</string>\n"
    "<key>spriteSourceSize</key>\n"
    "<string>{" << original_width << ", " << original_height << "}</string>\n"
    "<key>textureRect</key>\n"
    "<string>{{" << x << ", " << y
         << "}, {" << cropped_width << ", " << cropped_height << "}}</string>\n"
    "<key>textureRotated</key>\n"
         << ( sprite.rotated ? "<true/>\n" : "<false/>\n" )
         << "</dict>\n";

  return result.str();
}

std::string
pms::generators::plist::get_metadata_entry
( const std::string& base_name, const layout::atlas_page& desc ) const
{
  std::ostringstream result;
  
  result << "<key>metadata</key>\n"
    "<dict>\n"
    "<key>version</key>\n"
    "<string>1.6.0</string>\n"
    "<key>format</key>\n"
    "<integer>3</integer>\n"
    "<key>size</key>\n"
    "<string>{" << desc.width << ", " << desc.height << "}</string>\n"
    "<key>name</key>\n"
    "<string>" << base_name << "</string>\n"
    "<key>textureFileName</key>\n"
    "<string>" << base_name << ".png</string>\n"
    "<key>premultipliedAlpha</key>\n"
         << ( ( m_color_mode == color_mode::multiply_alpha )
              ? "<true/>\n"
              : "<false/>\n" )
         << "</dict>\n";

  return result.str();
}
