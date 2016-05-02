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
#ifndef __PMS_NODE_PARSER_FILE_HPP__
#define __PMS_NODE_PARSER_FILE_HPP__

#include "node_parser/node_parser.hpp"

#include "spritedesc.hpp"

namespace pms
{
  class image_map;

  class node_parser_file:
    public node_parser
  {
  public:
    void parse_node
    ( image_map& images, spritedesc& desc, const tree_node& node ) const;

  private:
    void process_sprite_sheet
    ( image_map& images, spritedesc& desc, const tree_node& node ) const;

  };
}

#endif
