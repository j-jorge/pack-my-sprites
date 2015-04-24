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
#include "node_parser/node_parser_xcf_entry.hpp"

#include "spritedesc.hpp"
#include "xcf_map.hpp"

#include <claw/assert.hpp>

void pms::node_parser_xcf_entry::parse_node
( xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  CLAW_PRECOND( node.children.size() == 2 );

  const std::string id
    ( node.children[0].value.begin(), node.children[0].value.end() );
  const std::string name
    ( node.children[1].value.begin(), node.children[1].value.end() );
  
  xcf.load( name );

  desc.xcf[ id ] = name;
}
