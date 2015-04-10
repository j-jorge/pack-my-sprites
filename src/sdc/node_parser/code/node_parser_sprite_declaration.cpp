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
#include "node_parser/node_parser_sprite_declaration.hpp"

#include "grammar.hpp"

#include <claw/string_algorithm.hpp>

void sdc::node_parser_sprite_declaration::parse_node
( const xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  // node 0 is the name
  // node 1 is the size
  // node 2 is the source image
  // node 3 is the layer list
  // node 4 is the mask (optional)

  spritedesc::sprite s;
  get_sprite_name( s, node.children[0] );

  if ( desc.get_sprite_by_name(s.name) != desc.sprite_end() )
    std::cerr << "warning: duplicate sprite entry '" << s.name << "'."
              << std::endl;

  get_image_id( s, node.children[2] );

  xcf_info image;
  get_xcf_from_id( xcf, desc, image, s.xcf_id );

  get_layers_and_size( image, s, node.children[1], node.children[3] );

  if ( node.children.size() == 5 )
    s.mask = get_layers( image, node.children[4] );

  desc.add_sprite( s );
}

void sdc::node_parser_sprite_declaration::get_sprite_name
( spritedesc::sprite& s, const tree_node& node ) const
{
  const std::string name( node.value.begin(), node.value.end() );

  claw::text::c_escape
    ( name.begin(), name.end(), std::inserter( s.name, s.name.end() ) );
}

void sdc::node_parser_sprite_declaration::get_image_id
( spritedesc::sprite& s, const tree_node& node ) const
{
  s.xcf_id = std::string( node.value.begin(), node.value.end() );
}

void sdc::node_parser_sprite_declaration::get_layers_and_size
( const xcf_info& image, spritedesc::sprite& s, const tree_node& size_node,
  const tree_node& layer_list_node ) const
{
  tree_node ratio;

  if ( size_node.value.id() == grammar::id_autosize )
    {
      s.layers = get_layers( image, layer_list_node );
      compute_source_box( s );
      ratio = size_node;
    }
  else
    {
      if ( size_node.value.id() == grammar::id_image_size )
        {
          s.source_box.set( 0, 0, image.width, image.height );
          ratio = size_node;
        }
      else // id == grammar::id_layer_size
        {
          layer_info layer;
          get_layer_info( image, layer, size_node.children[0] );

          s.source_box = layer.box;
          ratio = size_node.children[1];
        }

      s.layers = get_layers( image, layer_list_node );
    }

  crop_sprite_to_image_bounds( image, s );
  apply_result_box_ratio( s, ratio );
}

std::list<sdc::layer_info> sdc::node_parser_sprite_declaration::get_layers
( const xcf_info& image, const tree_node& layer_list_node ) const
{
  std::list<layer_info> result;

  if ( (layer_list_node.value.id() == grammar::id_string)
       || (layer_list_node.value.id() == grammar::id_layer_properties)
       || (layer_list_node.value.id() == grammar::id_glob) )
    add_layers( image, result, layer_list_node );
  else
    for ( std::size_t i(0); i!=layer_list_node.children.size(); ++i )
      add_layers( image, result, layer_list_node.children[i] );

  return result;
}

void sdc::node_parser_sprite_declaration::apply_layer_properties
( std::list<layer_info>& result, const tree_node& properties_node ) const
{
  std::vector<std::string> properties;

  if ( properties_node.value.id() == grammar::id_layer_properties )
    for ( std::size_t i(0); i != properties_node.children.size(); ++i )
      {
        const std::string p
          ( properties_node.children[i].value.begin(),
            properties_node.children[i].value.end() );

        properties.push_back( p );
      }
  else
    properties.push_back
      ( std::string
        ( properties_node.value.begin(), properties_node.value.end() ) );

  for ( std::size_t i(0); i != properties.size(); ++i )
    if ( properties[i] == "hollow" )
      for ( std::list<layer_info>::iterator it=result.begin();
            it != result.end(); ++it )
        {
          it->box.width = 0;
          it->box.height = 0;
        }
    else
      std::cerr << "Unknown layer property: '" << properties[i] << "'."
                << std::endl;
}

void sdc::node_parser_sprite_declaration::add_layers
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_exclude )
    exclude_layers( image, result, node );
  else
    {
      tree_node layer_node;
      const bool has_properties
        ( (node.children.size() == 2)
          && (node.value.id() == grammar::id_layer_properties) );

      if ( has_properties )
        layer_node = node.children[1];
      else
        layer_node = node;

      std::list<layer_info> layers;
  
      if ( layer_node.value.id() == grammar::id_glob )
        add_layers_glob( image, layers, layer_node );
      else // layer_node.value.id() == grammar::id_string
        add_single_layer( image, layers, layer_node );

      if ( has_properties )
        apply_layer_properties( layers, node.children[0] );

      std::copy( layers.begin(), layers.end(), std::back_inserter(result) );
    }
}

void sdc::node_parser_sprite_declaration::add_layers_glob
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_glob );
  CLAW_PRECOND( (node.children.size() == 2) || (node.children.size() == 3));

  const std::string pattern
      ( node.children.back().value.begin(), node.children.back().value.end() );

  xcf_info::layer_map::const_iterator it;
  bool found=false;

  for ( it=image.layers.begin(); it!=image.layers.end(); ++it )
    if ( claw::text::glob_match( pattern, it->first ) )
      {
        result.push_back( it->second );
        found = true;
      }

  if ( !found )
    std::cerr << "Unmatched pattern '" << pattern << "'" << std::endl;
}

void sdc::node_parser_sprite_declaration::exclude_layers
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_exclude );
  CLAW_PRECOND( node.children.size() == 2 );

  std::list<layer_info> excluded;
  add_layers( image, excluded, node.children[1] );

  for ( std::list<layer_info>::iterator it( excluded.begin() );
        it != excluded.end(); ++it )
    {
      std::list<layer_info>::iterator candidate( result.begin() );
      bool found(false);

      while ( !found && (candidate != result.end()) )
        if ( candidate->index == it->index )
          {
            found = true;
            result.erase( candidate );
          }
        else
          ++candidate;
    }
}

void sdc::node_parser_sprite_declaration::add_single_layer
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  layer_info info;
        
  get_layer_info( image, info, node );
  result.push_back( info );
}

void sdc::node_parser_sprite_declaration::compute_source_box
( spritedesc::sprite& s ) const
{
  std::list<layer_info>::const_iterator it;
  bool initialized( false );

  for ( it = s.layers.begin(); !initialized && (it != s.layers.end()); ++it )
    if ( it->box.area() != 0 )
      {
        s.source_box = it->box;
        initialized = true;
      }
     
  if ( !initialized )
    {
      std::cerr << "Cannot compute the size of the sprite '"
                << s.name << "' with " << s.layers.size() << " layer(s)."
                << " Check that you have at least one layer without the "
        "'hollow' property." << std::endl;
      return;
    }

  for ( ; it != s.layers.end(); ++it )
    if ( it->box.area() != 0 )
      s.source_box = s.source_box.join( it->box );
}

void sdc::node_parser_sprite_declaration::get_layer_info
( const xcf_info& xcf, layer_info& layer, const tree_node& node ) const
{
  std::string raw_name( node.value.begin(), node.value.end() );
  std::string layer_name;
  claw::text::c_escape
    ( raw_name.begin(), raw_name.end(),
      std::inserter( layer_name, layer_name.end() ) );

  const xcf_info::layer_map::const_iterator it_layer =
    xcf.layers.find( layer_name );

  if ( it_layer == xcf.layers.end() )
    std::cerr << "Unknown layer '" << layer_name << "'" << std::endl;
  else
    layer = it_layer->second;
}
