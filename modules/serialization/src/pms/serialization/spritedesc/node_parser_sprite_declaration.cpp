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
#include "pms/serialization/spritedesc/node_parser_sprite_declaration.hpp"

#include "pms/serialization/spritedesc/grammar.hpp"

#include <claw/string_algorithm.hpp>

void pms::serialization::spritedesc::node_parser_sprite_declaration::parse_node
( const resources::image_mapping& images, layout::description& desc,
  const tree_node& node ) const
{
  layout::description::sprite s;
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

  boost::optional< const resources::image& > image
    ( get_image_from_id( images, desc, s.image_id ) );

  if ( !image )
    return;
  
  get_layers_and_size( *image, s, size_node, layers_node );

  if ( n < node.children.size() )
    s.mask = get_layers( *image, node.children[n++] );

  desc.add_sprite( s );
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration
::apply_sprite_properties
( layout::description::sprite& result, const tree_node& properties_node ) const
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
pms::serialization::spritedesc::node_parser_sprite_declaration::get_properties
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

void
pms::serialization::spritedesc::node_parser_sprite_declaration::get_sprite_name
( layout::description::sprite& s, const tree_node& node ) const
{
  const std::string name( node.value.begin(), node.value.end() );

  claw::text::c_escape
    ( name.begin(), name.end(), std::inserter( s.name, s.name.end() ) );
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration::get_image_id
( layout::description::sprite& s, const tree_node& node ) const
{
  s.image_id = std::string( node.value.begin(), node.value.end() );
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration
::get_layers_and_size
( const resources::image& image, layout::description::sprite& s,
  const tree_node& size_node, const tree_node& layer_list_node ) const
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
          resources::layer layer;
          get_layer_info( image, layer, key );

          s.source_box.set( layer.x, layer.y, layer.width, layer.height );
        }

      s.layers = get_layers( image, layer_list_node );
    }

  crop_sprite_to_image_bounds( image, s );

  if ( size_node.children.empty() )
    apply_result_box_ratio( s, 1 );
  else
    apply_result_box_ratio( s, size_node.children[ 1 ] );
}

std::vector<pms::resources::layer>
pms::serialization::spritedesc::node_parser_sprite_declaration::get_layers
( const resources::image& image, const tree_node& layer_list_node ) const
{
  std::vector<resources::layer> result;

  if ( (layer_list_node.value.id() == grammar::id_string)
       || (layer_list_node.value.id() == grammar::id_properties)
       || (layer_list_node.value.id() == grammar::id_glob) )
    add_layers( image, result, layer_list_node );
  else
    for ( std::size_t i(0); i!=layer_list_node.children.size(); ++i )
      add_layers( image, result, layer_list_node.children[i] );

  return result;
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration
::apply_layer_properties
( std::vector<resources::layer>& result,
  const tree_node& properties_node ) const
{
  const std::vector<std::string> properties
    ( get_properties( properties_node ) );

  for ( std::size_t i(0); i != properties.size(); ++i )
    if ( properties[i] == "hollow" )
      for ( std::vector<resources::layer>::iterator it=result.begin();
            it != result.end(); ++it )
        {
          it->width = 0;
          it->height = 0;
        }
    else
      std::cerr << "Unknown layer property: '" << properties[i] << "'."
                << std::endl;
}

void pms::serialization::spritedesc::node_parser_sprite_declaration::add_layers
( const resources::image& image, std::vector<resources::layer>& result,
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

      std::vector<resources::layer> layers;
  
      if ( layer_node.value.id() == grammar::id_glob )
        add_layers_glob( image, layers, layer_node );
      else // layer_node.value.id() == grammar::id_string
        add_single_layer( image, layers, layer_node );

      if ( has_properties )
        apply_layer_properties( layers, node.children[0] );

      std::copy( layers.begin(), layers.end(), std::back_inserter(result) );
    }
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration::add_layers_glob
( const resources::image& image, std::vector<resources::layer>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_glob );
  CLAW_PRECOND( (node.children.size() == 2) || (node.children.size() == 3));

  const std::string pattern
      ( node.children.back().value.begin(), node.children.back().value.end() );

  bool found( false );

  for ( const auto& e : image.layers )
    if ( claw::text::glob_match( pattern, e.first ) )
      {
        result.push_back( e.second );
        found = true;
      }

  if ( !found )
    std::cerr << "Unmatched pattern '" << pattern << "'" << std::endl;
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration::exclude_layers
( const resources::image& image, std::vector<resources::layer>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_exclude );
  CLAW_PRECOND( node.children.size() == 2 );

  std::vector<resources::layer> excluded;
  add_layers( image, excluded, node.children[1] );

  for ( std::vector<resources::layer>::iterator it( excluded.begin() );
        it != excluded.end(); ++it )
    {
      std::vector<resources::layer>::iterator candidate( result.begin() );
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

void
pms::serialization::spritedesc::node_parser_sprite_declaration::add_single_layer
( const resources::image& image, std::vector<resources::layer>& result,
  const tree_node& node ) const
{
  resources::layer info;
        
  get_layer_info( image, info, node );
  result.push_back( info );
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration
::compute_source_box
( layout::description::sprite& s ) const
{
  std::vector<resources::layer>::const_iterator it;
  bool initialized( false );

  for ( it = s.layers.begin(); !initialized && (it != s.layers.end()); ++it )
    if ( ( it->width != 0 ) && ( it->height != 0 ) )
      {
        s.source_box.set( it->x, it->y, it->width, it->height );
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
    if ( ( it->width != 0 ) && ( it->height != 0 ) )
      {
        const claw::math::rectangle< int > r
          ( it->x, it->y, it->width, it->height );
        s.source_box = s.source_box.join( r );
      }
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration::get_layer_info
( const resources::image& images, resources::layer& layer,
  const tree_node& node ) const
{
  return get_layer_info
    ( images, layer, std::string( node.value.begin(), node.value.end() ) );
}

void
pms::serialization::spritedesc::node_parser_sprite_declaration::get_layer_info
( const resources::image& images, resources::layer& layer,
  const std::string& raw_name ) const
{
  std::string layer_name;
  claw::text::c_escape
    ( raw_name.begin(), raw_name.end(),
      std::inserter( layer_name, layer_name.end() ) );

  const auto it( images.layers.find( layer_name ) );

  if ( it == images.layers.end() )
    std::cerr << "Unknown layer '" << layer_name << "'" << std::endl;
  else
    layer = it->second;
}
