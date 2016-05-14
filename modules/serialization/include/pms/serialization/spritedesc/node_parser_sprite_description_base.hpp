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

#include "pms/layout/description.hpp"
#include "pms/resources/image_mapping.hpp"
#include "pms/serialization/spritedesc/node_parser.hpp"

namespace pms
{
  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser_sprite_description_base:
        public node_parser
      {
      protected:
        void apply_result_box_ratio
        ( layout::description::sprite& s, const tree_node& node ) const;

        void apply_result_box_ratio
        ( layout::description::sprite& s, float ratio ) const;

        void crop_sprite_to_image_bounds
        ( const resources::image& image, layout::description::sprite& s ) const;

        boost::optional< const resources::image& > get_image_from_id
        ( const resources::image_mapping& images,
          const layout::description& desc,
          const std::string& image_id ) const;

      };
    }
  }
}
