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
#ifndef __PMS_NODE_PARSER_SPRITE_SHEET_HPP__
#define __PMS_NODE_PARSER_SPRITE_SHEET_HPP__

#include "node_parser/node_parser.hpp"

namespace pms
{
  class spritedesc;
  class image_map;

  class node_parser_sprite_sheet:
    public node_parser
  {
  public:
    void parse_node
      ( image_map& xcf, spritedesc& desc, const tree_node& node ) const;

  private:
    void parse_name( spritedesc& desc, const tree_node& node ) const;
    void parse_width( spritedesc& desc, const tree_node& node ) const;
    void parse_height( spritedesc& desc, const tree_node& node ) const;
    bool parse_margin( spritedesc& desc, const tree_node& node ) const;
    bool parse_order( spritedesc& desc, const tree_node& node ) const;

    void check_xcf_usage( const spritedesc& desc ) const;

  };
}

#endif
