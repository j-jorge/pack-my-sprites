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

#include "pms/serialization/spritedesc/node_parser_sprite_description_base.hpp"

namespace pms
{
  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser_sprite_declaration:
        public node_parser_sprite_description_base
      {
      public:
        void parse_node
        ( const resources::image_mapping& images, layout::atlas_page& s,
          const tree_node& node ) const;

      private:
        void apply_sprite_properties
        ( layout::atlas_page::sprite& result,
          const tree_node& properties_node ) const;
        std::vector<std::string>
        get_properties( const tree_node& properties_node ) const;
    
        void get_sprite_name
        ( layout::atlas_page::sprite& s, const tree_node& node ) const;
        void get_image_id
        ( layout::atlas_page::sprite& s, const tree_node& node ) const;
        void get_layers_and_size
        ( const resources::image& image, layout::atlas_page::sprite& s,
          const tree_node& size_node, const tree_node& layer_list_node ) const;

        std::vector<resources::layer> get_layers
        ( const resources::image& image,
          const tree_node& layer_list_node ) const;

        void apply_layer_properties
        ( std::vector<resources::layer>& result,
          const tree_node& properties_node ) const;

        void add_layers
        ( const resources::image& image, std::vector<resources::layer>& result,
          const tree_node& node ) const;
        void add_layers_glob
        ( const resources::image& image, std::vector<resources::layer>& result,
          const tree_node& node ) const;
        void exclude_layers
        ( const resources::image& image, std::vector<resources::layer>& result,
          const tree_node& node ) const;
        void add_single_layer
        ( const resources::image& image, std::vector<resources::layer>& result,
          const tree_node& node ) const;

        void compute_source_box( layout::atlas_page::sprite& s ) const;

        void get_layer_info
        ( const resources::image& images, resources::layer& layer,
          const tree_node& node ) const;
        void get_layer_info
        ( const resources::image& images, resources::layer& layer,
          const std::string& raw_name ) const;

      };
    }
  }
}
