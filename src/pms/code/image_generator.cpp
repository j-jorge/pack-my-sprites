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
#include "image_generator.hpp"

#include <fstream>
#include <iostream>

#include <limits>

#include <claw/logger.hpp>
#include <claw/png.hpp>

pms::image_generator::image_generator( const gimp_interface& gimp )
  : m_gimp( gimp )
{

}

void pms::image_generator::generate
( const std::string& source, const sprite_sheet& sheet ) const
{
  const working_directory dir( source );

  claw::logger << claw::log_verbose
               << "Generating sprite sheet '"
               << sheet.description.output_name << "'";

  if ( sheet.internally_supported() )
    {
      claw::logger << claw::log_verbose << " using internal tool.\n";
      generate_output_with_internal_tool( dir, sheet.image, sheet.description );
    }
  else
    {
      claw::logger << claw::log_verbose << " using internal tool.\n";
      generate_output_with_gimp( dir, sheet.image, sheet.description );
    }
}

void pms::image_generator::generate_output_with_internal_tool
( const working_directory& dir, const image_map& images,
  const spritedesc& desc ) const
{
  claw::graphic::image result( desc.width, desc.height );
  result.fill
    ( claw::math::rectangle< int >( 0, 0, desc.width, desc.height ),
      claw::graphic::transparent_pixel );
  
  for ( spritedesc::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    copy_sprite( result, desc.images.find( it->image_id )->second, *it );
  
  claw::graphic::png::writer writer( result );
  std::ofstream output( dir.get_output_image_path( desc.output_name ) );
  writer.save( output );
}

void pms::image_generator::copy_sprite
( claw::graphic::image& result, const std::string& file_path,
  const spritedesc::sprite& sprite ) const
{
  std::ifstream f( file_path );
  claw::graphic::image image( f );

  if ( sprite.rotated )
    rotate( image );

  const claw::math::coordinate_2d< int > position( sprite.result_box.position );

  result.partial_copy( image, position );

  if ( sprite.bleed )
    bleed( result, image, position );
}

void pms::image_generator::rotate( claw::graphic::image& image ) const
{
  const unsigned int source_width( image.width() );
  const unsigned int source_height( image.height() );

  const unsigned int dest_width( source_height );
  const unsigned int dest_height( source_width );
  
  claw::graphic::image result( dest_width, dest_height );

  for ( unsigned int y( 0 ); y != dest_height; ++y )
    for ( unsigned int x( 0 ); x != dest_width; ++x )
      result[ y ][ x ] = image[ x ][ source_width - y - 1 ];

  image.swap( result );
}

void pms::image_generator::bleed
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

void pms::image_generator::generate_output_with_gimp
( const working_directory& dir, const image_map& images,
  const spritedesc& desc ) const
{
  std::ostringstream oss;
  generate_scm( oss, dir, images, desc );

  gimp_interface::path_list_type includes;
  includes.push_back( "common.scm" );

  const std::string result( m_gimp.run( oss.str(), includes ) );

  if ( result.find( " Error:" ) == std::string::npos )
    claw::logger << claw::log_verbose << result << '\n';
  else
    std::cerr << result << '\n';
}

void pms::image_generator::generate_scm
( std::ostream& os, const working_directory& dir, const image_map& images,
  const spritedesc& desc ) const
{
  os << "(let ( ";

  for ( spritedesc::id_to_file_map::const_iterator it = desc.images.begin();
        it != desc.images.end(); ++it )
    {
      const std::string image_path( dir.get_image_path( it->second ) );

      os << "(" << make_image_varname( it->first )
         << " (car (gimp-file-load 1 \"" << image_path << "\" \"" << image_path
         << "\" )))\n";
    }

  os << "(" << make_image_varname(desc.output_name)
     << " (new-image " << desc.width << ' ' << desc.height << "))\n";

  os << ")\n";

  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    generate_scm
      ( os, images.get_info( desc.images.find( it->image_id )->second ), *it,
        desc.output_name );

  os << "(save-frames \""
     << dir.get_output_image_path(desc.output_name) << "\" "
     << make_image_varname(desc.output_name) << ")\n";

  os << ")\n";
}

void pms::image_generator::generate_scm
( std::ostream& os, const image_info& images, const spritedesc::sprite& s,
  const std::string& target_id ) const
{
  // starting with version 3 of the format, the XCF files have layer groups.
  if ( images.version >= 3 )
    {
      os << "(create-layer-crop-with-groups " << make_image_varname(s.image_id)
         << " '(";

      for ( std::list<layer_info>::const_iterator it=s.layers.begin();
            it != s.layers.end(); ++it )
        os << '"' << images.get_layer_name( it->index ) << "\" ";
    }
  else
    {
      os << "(create-layer-crop " << make_image_varname(s.image_id) << " '(";

      for ( std::list<layer_info>::const_iterator it=s.layers.begin();
            it != s.layers.end(); ++it )
        os << it->index << ' ';
    }

  os << ") " << s.source_box.position.x << ' ' << s.source_box.position.y << ' '
     << s.source_box.width << ' ' << s.source_box.height << ' '
     << s.result_box.position.x << ' ' << s.result_box.position.y << ' '
     << s.result_box.width << ' ' << s.result_box.height;

  os << ' ' << ( s.rotated ? 1 : 0 )
     << ' ' << ( s.bleed ? 1 : 0 )
     << ' ' << make_image_varname(target_id) << " '(";

  if ( images.version >= 3 )
    {
      for ( std::list<layer_info>::const_iterator it=s.mask.begin();
            it != s.mask.end(); ++it )
        os << '"' << images.get_layer_name( it->index ) << "\" ";
    }
  else
    {
      for ( std::list<layer_info>::const_iterator it=s.mask.begin();
            it != s.mask.end(); ++it )
        os << it->index << ' ';
    }

  os << "))\n";
}

std::string
pms::image_generator::make_image_varname( const std::string& id ) const
{
  return id + "_image";
}
