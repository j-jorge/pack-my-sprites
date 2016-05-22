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
#include "pms/serialization/spritedesc/node_parser_file.hpp"

#include "pms/serialization/spritedesc/node_parser_atlas.hpp"

#include "pms/serialization/spritedesc/grammar.hpp"

void pms::serialization::spritedesc::node_parser_file::parse_node
( layout::atlas& atlas, const tree_node& node ) const
{
  assert( node.value.id() == grammar::id_atlas );
  process_atlas( atlas, node );
}

void pms::serialization::spritedesc::node_parser_file::process_atlas
( layout::atlas& atlas, const tree_node& node ) const
{
  node_parser_atlas parser;

  parser.parse_node( atlas, node );
}
