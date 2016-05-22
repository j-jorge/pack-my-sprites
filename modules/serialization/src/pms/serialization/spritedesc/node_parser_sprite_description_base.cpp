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
#include "pms/serialization/spritedesc/node_parser_sprite_description_base.hpp"

#include "pms/resources/image_mapping.hpp"

void
pms::serialization::spritedesc::node_parser_sprite_description_base
::apply_result_box_ratio
( layout::atlas_page::sprite& s, const tree_node& node ) const
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

void
pms::serialization::spritedesc::node_parser_sprite_description_base
::apply_result_box_ratio
( layout::atlas_page::sprite& s, float ratio ) const
{ 
  s.result_box.width = s.source_box.width * ratio;
  s.result_box.height = s.source_box.height * ratio;
}

void
pms::serialization::spritedesc::node_parser_sprite_description_base
::crop_sprite_to_image_bounds
( const resources::image& image, layout::atlas_page::sprite& s ) const
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

boost::optional< const pms::resources::image& >
pms::serialization::spritedesc::node_parser_sprite_description_base
::get_image_from_id
( const resources::image_mapping& images, const layout::atlas_page& desc,
  const std::string& image_id ) const
{
  // solve the image name from the identifier.
  const auto it( desc.images.find( image_id ));

  if ( it == desc.images.end() )
    {
      std::cerr << "Unknown image identifier '" << image_id << "'" << std::endl;
      return boost::none;
    }

  const std::string image_name( it->second );
  const boost::optional< const resources::image& > result
    ( images.get_image( image_name ) );
      
  if ( !result )
    std::cerr << "Unknown image file '" << image_name << "'" << std::endl;

  return result;
}
