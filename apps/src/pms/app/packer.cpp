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
#include "pms/app/packer.hpp"

#include "pms/generators/css.hpp"
#include "pms/generators/plist.hpp"
#include "pms/generators/png.hpp"
#include "pms/generators/spritepos.hpp"
#include "pms/layout/build.hpp"
#include "pms/serialization/read_spritedesc.hpp"

#include <claw/logger.hpp>

#include <boost/filesystem/convenience.hpp>

pms::app::packer::packer( const packer_options& options )
  : m_options( options ),
    m_gimp( m_options.scheme_directory, m_options.gimp_console_program )
  
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
  const boost::optional< layout::sprite_sheet > sheet
    ( generate_sprite_sheet( file ) );

  if ( !sheet )
    return false;
  
  if ( !run( file, *sheet ) )
    {
      std::cerr << "Failed to build sprite sheet '" << file << "'.\n";
      return false;
    }

  return true;
}

bool pms::app::packer::run
( const std::string& source_file_path, layout::sprite_sheet sheet ) const
{
  claw::logger << claw::log_verbose
               << "Generating sprite sheet:\n"
               << sheet.to_string()
               << " from file '" << source_file_path << "'\n";

  if ( !layout::build( m_options.enable_sprite_rotation, sheet.description ) )
    return false;
  
  generators::png generator( m_gimp );
  generator.generate( source_file_path, sheet );

  if ( m_options.generate_spritepos )
    {
      generators::spritepos generator;
      generator.generate( source_file_path, sheet );
    }

  if ( m_options.generate_plist )
    {
      generators::plist generator;
      generator.generate( source_file_path, sheet );
    }

  if ( m_options.generate_css )
    {
      generators::css generator;
      generator.generate( source_file_path, sheet );
    }

  return true;
}

boost::optional< pms::layout::sprite_sheet >
pms::app::packer::generate_sprite_sheet( const std::string& file_name ) const
{
  if ( file_name == "-" )
    return generate_sprite_sheet( ".", std::cin );
  
  std::ifstream f( file_name );

  if ( !f )
    {
      std::cerr << "Can't find file '" << file_name << "'." << std::endl;
      return boost::none;
    }

  const boost::filesystem::path file_path( file_name );
  const boost::filesystem::path file_directory( file_path.parent_path() );

  return generate_sprite_sheet( file_directory.string(), f );
}

pms::layout::sprite_sheet
pms::app::packer::generate_sprite_sheet
( const std::string& directory, std::istream& in ) const
{
  const resources::image_mapping images( directory, m_gimp );

  return layout::sprite_sheet
    ( serialization::read_spritedesc( images, in ) );
}