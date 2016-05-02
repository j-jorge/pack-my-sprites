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
#include "node_parser/node_parser_file.hpp"

#include "node_parser/node_parser_sprite_sheet.hpp"

#include "grammar.hpp"

void pms::node_parser_file::parse_node
( image_map& images, spritedesc& desc, const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_sprite_sheet );
  process_sprite_sheet( images, desc, node );
}

void pms::node_parser_file::process_sprite_sheet
( image_map& images, spritedesc& desc, const tree_node& node ) const
{
  node_parser_sprite_sheet parser;

  parser.parse_node( images, desc, node );
}
