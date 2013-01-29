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
/**
 * \file
 * \brief Compile the node of the "sprite_declaration" rule.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__
#define __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__

#include "node_parser/node_parser_sprite_description_base.hpp"

namespace sdc
{
  /**
   * \brief Compile the node of the "layer_to_sprite" rule.
   * \author Julien Jorge
   */
  class node_parser_layer_to_sprite:
    public node_parser_sprite_description_base
  {
  public:
    void parse_node
    ( const xcf_map& xcf, spritedesc& s, const tree_node& node ) const;

  }; // class node_parser_layer_to_sprite
} // namespace sdc

#endif // __SDC_NODE_PARSER_LAYER_TO_SPRITE_HPP__
