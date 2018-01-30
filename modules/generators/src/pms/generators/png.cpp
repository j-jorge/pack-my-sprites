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
#include "pms/generators/png.hpp"

#include "pms/generators/color_mode.hpp"
#include "pms/generators/rotation_direction.hpp"
#include "pms/generators/detail/working_directory.hpp"

#include <fstream>
#include <iostream>

#include <limits>

#include <claw/logger.hpp>
#include <claw/png.hpp>

pms::generators::png::png
( const gimp::system_interface& gimp, rotation_direction rotation,
  color_mode color )
  : m_rotation_direction( rotation ),
    m_color_mode( color ),
    m_gimp( gimp )
{

}

void pms::generators::png::generate
( const std::string& source, const layout::atlas& atlas ) const
{
  const detail::working_directory dir( source );

  claw::logger << claw::log_verbose
               << "Generating sprite sheet '"
               << atlas.output_name << "'";

  if ( atlas.internally_supported() )
    {
      claw::logger << claw::log_verbose << " using internal tool.\n";
      generate_output_with_internal_tool( dir, atlas );
    }
  else
    {
      claw::logger << claw::log_verbose << " using internal tool.\n";
      generate_output_with_gimp( dir, atlas );
    }
}

void pms::generators::png::generate_output_with_internal_tool
( const detail::working_directory& dir,
  const layout::atlas& atlas ) const
{
  const std::size_t page_count( atlas.pages.size() );

  for ( std::size_t i( 0 ); i != page_count; ++i )
    {
      claw::logger << claw::log_verbose
                   << "Page " << ( i + 1 ) << '/' << page_count << "…\n";

      generate_output_with_internal_tool( dir, i, atlas );
    }
}

void pms::generators::png::generate_output_with_internal_tool
( const detail::working_directory& dir, std::size_t index,
  const layout::atlas& atlas ) const
{
  const layout::atlas_page& desc( atlas.pages[ index ] );

  if ( ( desc.width == 0 ) || ( desc.height == 0 ) )
    return;
  
  claw::graphic::image result( desc.width, desc.height );

  std::fill( result.begin(), result.end(), claw::graphic::transparent_pixel );
  
  for ( layout::atlas_page::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    copy_sprite
      ( result,
        dir.get_image_path( desc.images.find( it->image_id )->second ),
        *it );

  if ( m_color_mode == color_mode::multiply_alpha )
    multiply_alpha( result );
  
  claw::graphic::png::writer writer( result );
  std::ofstream output
    ( dir.get_output_file_path
      ( atlas.output_name, index, atlas.pages.size(), "png" ) );
  writer.save( output );
}

void pms::generators::png::copy_sprite
( claw::graphic::image& result, const std::string& file_path,
  const layout::atlas_page::sprite& sprite ) const
{
  std::ifstream f( file_path );
  claw::graphic::image image( f );

  if ( ( sprite.display_offsets.width != sprite.result_box.width )
       || ( sprite.display_offsets.height != sprite.result_box.height ) )
    {
      claw::graphic::image part
        ( sprite.result_box.width, sprite.result_box.height );
      part.partial_copy( image, -sprite.display_offsets.position );
      image.swap( part );
    }
  
  if ( sprite.rotated )
    rotate( image );

  const claw::math::coordinate_2d< int > position( sprite.result_box.position );

  result.partial_copy( image, position );

  if ( sprite.bleed )
    bleed( result, image, position );
}

void pms::generators::png::rotate( claw::graphic::image& image ) const
{
  const unsigned int source_width( image.width() );
  const unsigned int source_height( image.height() );

  const unsigned int dest_width( source_height );
  const unsigned int dest_height( source_width );
  
  claw::graphic::image result( dest_width, dest_height );

  if ( m_rotation_direction == rotation_direction::clockwise )
    for ( unsigned int x( 0 ); x != dest_width; ++x )
      for ( unsigned int y( 0 ); y != dest_height; ++y )
        result[ y ][ x ] = image[ source_height - x - 1 ][ y ];
  else
    {
      assert( m_rotation_direction == rotation_direction::anticlockwise );
      
      for ( unsigned int y( 0 ); y != dest_height; ++y )
        for ( unsigned int x( 0 ); x != dest_width; ++x )
          result[ y ][ x ] = image[ x ][ source_width - y - 1 ];
    }

  image.swap( result );
}

void pms::generators::png::multiply_alpha( claw::graphic::image& image ) const
{
  static constexpr std::uint32_t range
    ( std::numeric_limits< claw::graphic::rgba_pixel::component_type >::max() );

  for ( claw::graphic::rgba_pixel& p : image )
    {
      const std::uint32_t alpha( p.components.alpha );
      
      p.components.red = p.components.red * alpha / range;
      p.components.green = p.components.green * alpha / range;
      p.components.blue = p.components.blue * alpha / range;
    }
}

void pms::generators::png::bleed
( claw::graphic::image& result, const claw::graphic::image& image,
  const claw::math::coordinate_2d< int >& position ) const
{
  const unsigned int width( image.width() );
  const unsigned int height( image.height() );
  
  for ( unsigned int y( 0 ); y != height; ++y )
    {
      result[ position.y + y ][ position.x - 1 ] = image[ y ][ 0 ];
      result[ position.y + y ][ position.x + width ] = image[ y ][ width - 1 ];
    }

  for ( unsigned int x( 0 ); x != width; ++x )
    {
      result[ position.y - 1 ][ position.x + x ] = image[ 0 ][ x ];
      result[ position.y + height ][ position.x + x ] =
        image[ height - 1 ][ x ];
    }

  result[ position.y - 1 ][ position.x - 1 ] = image[ 0 ][ 0 ];
  result[ position.y - 1 ][ position.x + width ] = image[ 0 ][ width - 1 ];
  result[ position.y + height ][ position.x - 1 ] = image[ height - 1 ][ 0 ];
  result[ position.y + height ][ position.x + width ] =
    image[ height - 1 ][ width - 1 ];
}

void pms::generators::png::generate_output_with_gimp
( const detail::working_directory& dir,
  const layout::atlas& atlas ) const
{
  const std::size_t page_count( atlas.pages.size() );

  for ( std::size_t i( 0 ); i != page_count; ++i )
    {
      claw::logger << claw::log_verbose
                   << "Page " << ( i + 1 ) << '/' << page_count << "…\n";

      generate_output_with_gimp( dir, i, atlas );
    }
}

void pms::generators::png::generate_output_with_gimp
( const detail::working_directory& dir, std::size_t index,
  const layout::atlas& atlas ) const
{
  std::ostringstream oss;
  generate_scm( oss, dir, index, atlas );

  gimp::system_interface::path_list_type includes;
  includes.push_back( "common.scm" );

  const std::string result( m_gimp.run( oss.str(), includes ) );

  if ( result.find( " Error:" ) == std::string::npos )
    claw::logger << claw::log_verbose << result << '\n';
  else
    std::cerr << result << '\n';
}

void pms::generators::png::generate_scm
( std::ostream& os, const detail::working_directory& dir, std::size_t index,
  const layout::atlas& atlas ) const
{
  const layout::atlas_page& desc( atlas.pages[ index ] );
  
  os << "(let ( ";

  for ( auto it( desc.images.begin() ); it != desc.images.end(); ++it )
    {
      const std::string image_path( dir.get_image_path( it->second ) );

      os << "(" << make_image_varname( it->first )
         << " (car (gimp-file-load 1 \"" << image_path << "\" \"" << image_path
         << "\" )))\n";
    }

  os << "(" << make_image_varname(atlas.output_name)
     << " (new-image " << desc.width << ' ' << desc.height << "))\n";

  os << ")\n";

  for ( auto it( desc.sprite_begin() ); it != desc.sprite_end(); ++it )
    generate_scm
      ( os, *atlas.image.get_image( desc.images.find( it->image_id )->second ),
        *it, atlas.output_name );

  os << "(save-frames \""
     << dir.get_output_file_path
          (atlas.output_name, index, atlas.pages.size(), "png")
     << "\" "
     << make_image_varname(atlas.output_name) << ")\n";

  os << ")\n";
}

void pms::generators::png::generate_scm
( std::ostream& os, const resources::image& image,
  const layout::atlas_page::sprite& s, const std::string& target_id ) const
{
  // starting with version 3 of the format, the XCF files have layer groups.
  if ( image.version >= 3 )
    {
      os << "(create-layer-crop-with-groups " << make_image_varname(s.image_id)
         << " '(";

      for ( auto it( s.layers.begin() ); it != s.layers.end(); ++it )
        os << '"' << image.get_layer_name( it->index ) << "\" ";
    }
  else
    {
      os << "(create-layer-crop " << make_image_varname(s.image_id) << " '(";

      for ( auto it( s.layers.begin() ); it != s.layers.end(); ++it )
        os << it->index << ' ';
    }

  os << ") " << s.source_box.position.x << ' ' << s.source_box.position.y << ' '
     << s.source_box.width << ' ' << s.source_box.height << ' '
     << s.result_box.position.x << ' ' << s.result_box.position.y << ' '
     << s.result_box.width << ' ' << s.result_box.height;

  os << ' ' << ( s.rotated ? 1 : 0 )
     << ' ' << ( s.bleed ? 1 : 0 )
     << ' ' << make_image_varname(target_id) << " '(";

  if ( image.version >= 3 )
    for ( auto it( s.mask.begin() ); it != s.mask.end(); ++it )
      os << '"' << image.get_layer_name( it->index ) << "\" ";
  else
    for ( auto it( s.mask.begin() ); it != s.mask.end(); ++it )
      os << it->index << ' ';

  os << "))\n";
}

std::string
pms::generators::png::make_image_varname( const std::string& id ) const
{
  return id + "_image";
}
