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
#pragma once

#include "pms/serialization/spritedesc/node_parser.hpp"

namespace pms
{
  namespace layout
  {
    class description;
    class sprite_sheet;
  }

  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser_sprite_sheet:
        public node_parser
      {
      public:
        void parse_node
        ( layout::sprite_sheet& sheet, const tree_node& node ) const;

      private:
        void parse_name
        ( layout::description& desc, const tree_node& node ) const;
        void parse_width
        ( layout::description& desc, const tree_node& node ) const;
        void parse_height
        ( layout::description& desc, const tree_node& node ) const;
        bool parse_margin
        ( layout::description& desc, const tree_node& node ) const;

        void check_image_usage( const layout::description& desc ) const;
      };
    }
  }
}
