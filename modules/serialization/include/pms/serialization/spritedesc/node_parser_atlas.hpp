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
#pragma once

#include "pms/serialization/spritedesc/node_parser.hpp"

namespace pms
{
  namespace layout
  {
    class atlas_page;
    class atlas;
  }

  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser_atlas:
        public node_parser
      {
      public:
        void parse_node
        ( layout::atlas& atlas, const tree_node& node ) const;

      private:
        void parse_name
        ( layout::atlas& atlas, const tree_node& node ) const;
        void parse_width
        ( layout::atlas& atlas, const tree_node& node ) const;
        void parse_height
        ( layout::atlas& atlas, const tree_node& node ) const;
        bool parse_margin
        ( layout::atlas& atlas, const tree_node& node ) const;

        void check_image_usage
        ( const std::string& output_name,
          const layout::atlas_page& desc ) const;
      };
    }
  }
}
