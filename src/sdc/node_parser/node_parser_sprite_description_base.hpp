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
 * \brief Base class for the nodes describing a sprite.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__
#define __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__

#include "node_parser/node_parser.hpp"

#include "xcf_map.hpp"
#include "spritedesc.hpp"

namespace sdc
{
  /**
   * \brief Base class for the nodes describing a sprite.
   * \author Julien Jorge
   */
  class node_parser_sprite_description_base:
    public node_parser
  {
  protected:
    void apply_result_box_ratio
    ( spritedesc::sprite& s, const tree_node& node ) const;

    void crop_sprite_to_image_bounds
    ( const xcf_info& image, spritedesc::sprite& s ) const;

    void get_xcf_from_id
    ( const xcf_map& xcf, const spritedesc& desc, xcf_info& info,
      const std::string& xcf_id ) const;

  }; // class node_parser_sprite_description_base
} // namespace sdc

#endif // __SDC_NODE_PARSER_SPRITE_DESCRIPTION_BASE_HPP__
