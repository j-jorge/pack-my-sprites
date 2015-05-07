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
#include "node_parser/node_parser_sprite_description_base.hpp"

#include "xcf_map.hpp"

void pms::node_parser_sprite_description_base::apply_result_box_ratio
( spritedesc::sprite& s, const tree_node& node ) const
{ 
  const std::string input( node.value.begin(), node.value.end() );
  std::istringstream iss( input );
  double ratio;

  if ( iss >> ratio )
    apply_result_box_ratio( s, ratio );
  else
    std::cerr << "invalid ratio '" << input
              << "' for sprite '" << s.name << "'" << std::endl;
}

void pms::node_parser_sprite_description_base::apply_result_box_ratio
( spritedesc::sprite& s, float ratio ) const
{ 
  s.result_box.width = s.source_box.width * ratio;
  s.result_box.height = s.source_box.height * ratio;
}

void pms::node_parser_sprite_description_base::crop_sprite_to_image_bounds
( const xcf_info& image, spritedesc::sprite& s ) const
{
  if ( s.source_box.position.x < 0 )
    {
      s.source_box.width += s.source_box.position.x;
      s.source_box.position.x = 0;
    }

  if ( s.source_box.position.y < 0 )
    {
      s.source_box.height += s.source_box.position.y;
      s.source_box.position.y = 0;
    }

  if ( s.source_box.position.x + s.source_box.width > (int)image.width )
    s.source_box.width = image.width - s.source_box.position.x;

  if ( s.source_box.position.y + s.source_box.height > (int)image.height )
    s.source_box.height = image.height - s.source_box.position.y;
}

void pms::node_parser_sprite_description_base::get_xcf_from_id
( const xcf_map& xcf, const spritedesc& desc, xcf_info& info,
  const std::string& xcf_id ) const
{
  // solve the xcf name from the identifier.
  const std::map<std::string, std::string>::const_iterator it =
    desc.xcf.find( xcf_id );

  if ( it == desc.xcf.end() )
    std::cerr << "Unknown xcf identifier '" << xcf_id
              << "'" << std::endl;
  else
    {
      const std::string xcf_name = it->second;

      if ( !xcf.has_info( xcf_name ) )
        std::cerr << "Unknown xcf file '" << xcf_name << "'" << std::endl;
      else
        info = xcf.get_info( xcf_name );
    }
}
