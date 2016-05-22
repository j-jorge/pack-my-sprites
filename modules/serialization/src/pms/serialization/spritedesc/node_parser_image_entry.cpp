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
#include "pms/serialization/spritedesc/node_parser_image_entry.hpp"

#include "pms/layout/atlas_page.hpp"
#include "pms/resources/image_mapping.hpp"

void pms::serialization::spritedesc::node_parser_image_entry::parse_node
( resources::image_mapping& images, layout::atlas_page& desc,
  const tree_node& node ) const
{
  assert( node.children.size() == 2 );

  const std::string id
    ( node.children[0].value.begin(), node.children[0].value.end() );
  const std::string name
    ( node.children[1].value.begin(), node.children[1].value.end() );
  
  images.load( name );

  desc.images[ id ] = name;
}
