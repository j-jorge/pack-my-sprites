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
#include "pms/app/packer.hpp"

#include "pms/app/atlas_format.hpp"
#include "pms/generators/css.hpp"
#include "pms/generators/plist.hpp"
#include "pms/generators/png.hpp"
#include "pms/generators/rotation_direction.hpp"
#include "pms/generators/spine.hpp"
#include "pms/generators/spritepos.hpp"
#include "pms/layout/build.hpp"
#include "pms/serialization/read_spritedesc.hpp"

#include <claw/logger.hpp>

#include <boost/filesystem/convenience.hpp>

pms::app::packer::packer( const packer_options& options )
  : m_options( options ),
    m_gimp
    ( m_options.get_scheme_directory(), m_options.get_gimp_console_program() )
  
{
  
}

bool pms::app::packer::run( const std::vector< std::string >& files ) const
{
  bool result( true );
  
  for ( const std::string& file : files )
    if ( !run( file ) )
      result = false;

  return result;
}

bool pms::app::packer::run( const std::string& file ) const
{
  const boost::optional< layout::atlas > atlas
    ( generate_atlas( file ) );

  if ( !atlas )
    return false;
  
  if ( !run( file, *atlas ) )
    {
      std::cerr << "Failed to build sprite sheet '" << file << "'.\n";
      return false;
    }

  return true;
}

bool pms::app::packer::run
( const std::string& source_file_path, layout::atlas atlas ) const
{
  claw::logger << claw::log_verbose
               << "Generating sprite sheet:\n"
               << atlas.to_string()
               << " from file '" << source_file_path << "'\n";

  if ( !feasible( atlas ) )
    return false;

  const bool allow_rotation
    ( m_options.get_rotation_direction()
      != generators::rotation_direction::none );
  
  if ( !layout::build( allow_rotation, atlas ) )
    return false;
  
  generate( source_file_path, atlas );
  
  return true;
}

bool pms::app::packer::feasible( const layout::atlas& atlas ) const
{
  const std::size_t margin( atlas.margin );
  const std::size_t width( atlas.width );
  const std::size_t height( atlas.height );
  
  const bool allow_rotation
    ( m_options.get_rotation_direction()
      != generators::rotation_direction::none );

  for ( const layout::atlas_page& p : atlas.pages )
    for ( layout::atlas_page::const_sprite_iterator it( p.sprite_begin() );
          it != p.sprite_end(); ++it )
      {
        const std::size_t w( it->result_box.width + 2 * margin + it->bleed );
        const std::size_t h( it->result_box.height + 2 * margin + it->bleed );
        
        if ( ( ( w > width ) || ( h > height ) )
             && ( allow_rotation && ( ( w > height ) || ( h > width ) ) ) )
          {
            claw::logger << claw::log_error
                         << "Sprite '" << it->name
                         << "' is larger than the maximum page dimensions.\n";
            return false;
          }
      }

  return true;
}

void pms::app::packer::generate
( const std::string& source_file_path, const layout::atlas& atlas ) const
{
  generators::png generator
    ( m_gimp, m_options.get_rotation_direction(), m_options.get_color_mode() );
  generator.generate( source_file_path, atlas );

  switch( m_options.get_atlas_format() )
    {
    case atlas_format::spritepos:
      {
        generators::spritepos generator;
        generator.generate( source_file_path, atlas );
        break;
      }
    case atlas_format::spine:
      {
        generators::spine generator;
        generator.generate( source_file_path, atlas );
        break;
      }
    case atlas_format::plist:
      {
        generators::plist generator( m_options.get_color_mode() );
        generator.generate( source_file_path, atlas );
        break;
      }
    case atlas_format::css:
      {
        generators::css generator;
        generator.generate( source_file_path, atlas );
        break;
      }
    case atlas_format::none:
      break;
    }
}

boost::optional< pms::layout::atlas >
pms::app::packer::generate_atlas( const std::string& file_name ) const
{
  if ( file_name == "-" )
    return generate_atlas( ".", std::cin );
  
  std::ifstream f( file_name );

  if ( !f )
    {
      std::cerr << "Can't find file '" << file_name << "'." << std::endl;
      return boost::none;
    }

  const boost::filesystem::path file_path( file_name );
  const boost::filesystem::path file_directory( file_path.parent_path() );

  return generate_atlas( file_directory.string(), f );
}

pms::layout::atlas
pms::app::packer::generate_atlas
( const std::string& directory, std::istream& in ) const
{
  const resources::image_mapping images
    ( directory, m_gimp, m_options.should_crop() );

  return layout::atlas
    ( serialization::read_spritedesc( images, in ) );
}
