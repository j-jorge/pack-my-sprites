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
#include "plist_generator.hpp"

#include <fstream>

#include <claw/logger.hpp>

void sdc::plist_generator::generate
( std::string spritedesc_file_path, spritedesc_collection desc ) const
{
  const working_directory dir( spritedesc_file_path );
  typedef spritedesc_collection::spritedesc_list_type::iterator iterator_type;

  for ( iterator_type it=desc.sprite_sheet.begin();
        it!=desc.sprite_sheet.end(); ++it )
    {
      claw::logger << claw::log_verbose
                   << "Generating plist file for sprite sheet '"
                   << it->output_name << "' of '" << spritedesc_file_path << "'"
                   << std::endl;

      generate_plist( dir, *it );
    }
}

void sdc::plist_generator::generate_plist
( working_directory dir, spritedesc desc ) const
{
  const std::string filename
    ( dir.get_output_file_path( desc.output_name, "plist" ) );

  std::ofstream f( filename.c_str() );
  generate_plist( f, desc );
}

void sdc::plist_generator::generate_plist
( std::ostream& os, spritedesc desc ) const
{
  os << get_plist_header()
     << get_plist_body( desc );
}

std::string sdc::plist_generator::get_plist_header() const
{
  return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\""
    " \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
}

std::string sdc::plist_generator::get_plist_body( spritedesc desc ) const
{
  return std::string( "<plist version=\"1.0\">\n" )
    + "<dict>\n"
    + get_frames_entry( desc )
    + get_metadata_entry( desc )
    + "</dict>\n"
    + "</plist>\n";
}

std::string sdc::plist_generator::get_frames_entry( spritedesc desc ) const
{
  std::string result( "<key>frames</key>\n" );

  result += "<dict>\n";

  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    result += get_sprite_entry( *it );

  result += "</dict>\n";

  return result;
}

std::string
sdc::plist_generator::get_sprite_entry( spritedesc::sprite sprite ) const
{
  const int x( sprite.result_box.position.x );
  const int y( sprite.result_box.position.y );
  const int width( sprite.result_box.width );
  const int height( sprite.result_box.height );

  std::ostringstream result;

  result << "<key>" << sprite.name << "</key>\n"
    "<dict>\n"
    "<key>aliases</key>\n"
    "<array/>\n"
    "<key>spriteColorRect</key>\n"
    "<string>{{0, 0}, {" << width << ", " << height << "}}</string>\n"
    "<key>spriteOffset</key>\n"
    "<string>{0, 0}</string>\n"
    "<key>spriteSize</key>\n"
    "<string>{" << width << ", " << height << "}</string>\n"
    "<key>spriteSourceSize</key>\n"
    "<string>{" << width << ", " << height << "}</string>\n"
    "<key>spriteTrimmed</key>\n"
    "<true/>\n"
    "<key>textureRect</key>\n"
    "<string>{{" << x << ", " << y
         << "}, {" << width << ", " << height << "}}</string>\n"
    "<key>textureRotated</key>\n"
    "<false/>\n"
    "</dict>\n";

  return result.str();
}

std::string sdc::plist_generator::get_metadata_entry( spritedesc desc ) const
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
    "<string>" << desc.output_name << "</string>\n"
    "<key>textureFileName</key>\n"
    "<string>" << desc.output_name << ".png</string>\n"
    "<key>premultipliedAlpha</key>\n"
    "<false/>\n"
    "</dict>\n";

  return result.str();
}
