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

void pms::node_parser_sprite_declaration::parse_node
( const image_map& images, spritedesc& desc, const tree_node& node ) const
{
  spritedesc::sprite s;
  s.bleed = false;

  std::size_t n( 0 );

  if ( ( node.children[n].value.id() == grammar::id_properties )
       || ( node.children[n].value.id() == grammar::id_identifier ) )
      apply_sprite_properties( s, node.children[ n++ ] );

  const tree_node name_node( node.children[n++] );
  const tree_node size_node( node.children[n++] );
  const tree_node image_node( node.children[n++] );
  const tree_node layers_node( node.children[n++] );
  
  get_sprite_name( s, name_node );

  if ( desc.get_sprite_by_name(s.name) != desc.sprite_end() )
    std::cerr << "warning: duplicate sprite entry '" << s.name << "'."
              << std::endl;

  get_image_id( s, image_node );

  image_info image;
  get_image_from_id( images, desc, image, s.image_id );

  get_layers_and_size( image, s, size_node, layers_node );

  if ( n < node.children.size() )
    s.mask = get_layers( image, node.children[n++] );

  desc.add_sprite( s );
}

void pms::node_parser_sprite_declaration::apply_sprite_properties
( spritedesc::sprite& result, const tree_node& properties_node ) const
{
  const std::vector<std::string> properties
    ( get_properties( properties_node ) );

  for ( std::size_t i(0); i != properties.size(); ++i )
    if ( properties[i] == "bleed" )
      result.bleed = true;
    else
      std::cerr << "Unknown sprite property: '" << properties[i] << "'."
                << std::endl;
}

std::vector<std::string>
pms::node_parser_sprite_declaration::get_properties
( const tree_node& properties_node ) const
{
  std::vector<std::string> properties;

  if ( properties_node.value.id() == grammar::id_properties )
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

  return properties;
}

void pms::node_parser_sprite_declaration::get_sprite_name
( spritedesc::sprite& s, const tree_node& node ) const
{
  const std::string name( node.value.begin(), node.value.end() );

  claw::text::c_escape
    ( name.begin(), name.end(), std::inserter( s.name, s.name.end() ) );
}

void pms::node_parser_sprite_declaration::get_image_id
( spritedesc::sprite& s, const tree_node& node ) const
{
  s.image_id = std::string( node.value.begin(), node.value.end() );
}

void pms::node_parser_sprite_declaration::get_layers_and_size
( const image_info& image, spritedesc::sprite& s, const tree_node& size_node,
  const tree_node& layer_list_node ) const
{
  tree_node key_node;

  if ( size_node.children.empty() )
    key_node = size_node;
  else
    key_node = size_node.children[0];
  
  const std::string key( key_node.value.begin(), key_node.value.end() );
  
  if ( key == "autosize" )
    {
      s.layers = get_layers( image, layer_list_node );
      compute_source_box( s );
    }
  else
    {
      if ( key == "image_size" )
          s.source_box.set( 0, 0, image.width, image.height );
      else // key is the name of a layer
        {
          layer_info layer;
          get_layer_info( image, layer, key );

          s.source_box = layer.box;
        }

      s.layers = get_layers( image, layer_list_node );
    }

  crop_sprite_to_image_bounds( image, s );

  if ( size_node.children.empty() )
    apply_result_box_ratio( s, 1 );
  else
    apply_result_box_ratio( s, size_node.children[ 1 ] );
}

std::list<pms::layer_info> pms::node_parser_sprite_declaration::get_layers
( const image_info& image, const tree_node& layer_list_node ) const
{
  std::list<layer_info> result;

  if ( (layer_list_node.value.id() == grammar::id_string)
       || (layer_list_node.value.id() == grammar::id_properties)
       || (layer_list_node.value.id() == grammar::id_glob) )
    add_layers( image, result, layer_list_node );
  else
    for ( std::size_t i(0); i!=layer_list_node.children.size(); ++i )
      add_layers( image, result, layer_list_node.children[i] );

  return result;
}

void pms::node_parser_sprite_declaration::apply_layer_properties
( std::list<layer_info>& result, const tree_node& properties_node ) const
{
  const std::vector<std::string> properties
    ( get_properties( properties_node ) );

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

void pms::node_parser_sprite_declaration::add_layers
( const image_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_exclude )
    exclude_layers( image, result, node );
  else
    {
      tree_node layer_node;
      const bool has_properties
        ( (node.children.size() == 2)
          && (node.value.id() == grammar::id_properties) );

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

void pms::node_parser_sprite_declaration::add_layers_glob
( const image_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_glob );
  CLAW_PRECOND( (node.children.size() == 2) || (node.children.size() == 3));

  const std::string pattern
      ( node.children.back().value.begin(), node.children.back().value.end() );

  image_info::layer_map::const_iterator it;
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

void pms::node_parser_sprite_declaration::exclude_layers
( const image_info& image, std::list<layer_info>& result,
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

void pms::node_parser_sprite_declaration::add_single_layer
( const image_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  layer_info info;
        
  get_layer_info( image, info, node );
  result.push_back( info );
}

void pms::node_parser_sprite_declaration::compute_source_box
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

void pms::node_parser_sprite_declaration::get_layer_info
( const image_info& images, layer_info& layer, const tree_node& node ) const
{
  return get_layer_info
    ( images, layer, std::string( node.value.begin(), node.value.end() ) );
}

void pms::node_parser_sprite_declaration::get_layer_info
( const image_info& images, layer_info& layer,
  const std::string& raw_name ) const
{
  std::string layer_name;
  claw::text::c_escape
    ( raw_name.begin(), raw_name.end(),
      std::inserter( layer_name, layer_name.end() ) );

  const image_info::layer_map::const_iterator it_layer =
    images.layers.find( layer_name );

  if ( it_layer == images.layers.end() )
    std::cerr << "Unknown layer '" << layer_name << "'" << std::endl;
  else
    layer = it_layer->second;
}
