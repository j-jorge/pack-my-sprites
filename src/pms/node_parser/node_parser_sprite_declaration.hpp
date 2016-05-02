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
#ifndef __PMS_NODE_PARSER_SPRITE_DECLARATION_HPP__
#define __PMS_NODE_PARSER_SPRITE_DECLARATION_HPP__

#include "node_parser/node_parser_sprite_description_base.hpp"

namespace pms
{
  class node_parser_sprite_declaration:
    public node_parser_sprite_description_base
  {
  public:
    void parse_node
    ( const image_map& xcf, spritedesc& s, const tree_node& node ) const;

  private:
    void apply_sprite_properties
    ( spritedesc::sprite& result, const tree_node& properties_node ) const;
    std::vector<std::string>
    get_properties( const tree_node& properties_node ) const;
    
    void get_sprite_name
    ( spritedesc::sprite& s, const tree_node& node ) const;
    void get_image_id
    ( spritedesc::sprite& s, const tree_node& node ) const;
    void get_layers_and_size
    ( const image_info& image, spritedesc::sprite& s,
      const tree_node& size_node, const tree_node& layer_list_node ) const;

    std::list<layer_info> get_layers
    ( const image_info& image, const tree_node& layer_list_node ) const;

    void apply_layer_properties
    ( std::list<layer_info>& result, const tree_node& properties_node ) const;

    void add_layers
    ( const image_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void add_layers_glob
    ( const image_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void exclude_layers
    ( const image_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;
    void add_single_layer
    ( const image_info& image, std::list<layer_info>& result,
      const tree_node& node ) const;

    void compute_source_box( spritedesc::sprite& s ) const;

    void get_layer_info
    ( const image_info& xcf, layer_info& layer, const tree_node& node ) const;
    void get_layer_info
    ( const image_info& xcf, layer_info& layer,
      const std::string& raw_name ) const;

  };
}

#endif
