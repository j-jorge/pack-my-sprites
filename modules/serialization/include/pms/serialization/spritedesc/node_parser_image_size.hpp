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

#include "pms/layout/description.hpp"

namespace pms
{
  namespace resources
  {
    class image_info;
  }
  
  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser_image_size:
        public node_parser
      {
      public:
        void parse_node
        ( layout::description::sprite& s, const tree_node& node ) const;

      };
    }
  }
}
