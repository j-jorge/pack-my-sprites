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
#include "pms/resources/image_mapping.hpp"

#include "pms/files/is_fully_qualified.hpp"

#include <claw/exception.hpp>
#include <claw/image.hpp>
#include <claw/logger.hpp>

#include <fstream>
#include <sstream>
#include <utility>
#include <tuple>

pms::resources::image_mapping::image_mapping()
  : m_image_directory( "." )
{

}

pms::resources::image_mapping::image_mapping
( const std::string& image_directory, const gimp::system_interface& gimp )
  : m_image_directory( image_directory ), m_gimp( gimp )
{
  if ( m_image_directory.empty() )
    m_image_directory = '.';
}

void pms::resources::image_mapping::load( const std::string& name )
{
  if ( get_image(name) )
    return;

  std::string file_path;

  if ( files::is_fully_qualified( name ) )
    file_path = name;
  else
    file_path = m_image_directory + '/' + name;

  claw::logger << claw::log_verbose << "Loading '" << name << "' from '"
               << file_path << "'…" << std::endl;

  if ( load_with_internal_tool( name, file_path ) )
    return;

  load_with_gimp( name, file_path );
}

bool pms::resources::image_mapping::load_with_internal_tool
( const std::string& name, const std::string& file_path )
{
  claw::logger << claw::log_verbose << "Using internal tool…\n";

  std::ifstream f( file_path );

  if ( !f )
    return false;
  
  claw::graphic::image image_data;

  try
    {
      claw::graphic::image( f ).swap( image_data );
    }
  catch( const claw::bad_format& f )
    {
      return false;
    }

  image& result
    ( m_images.emplace
      ( std::piecewise_construct,
        std::forward_as_tuple( name ),
        std::forward_as_tuple( "Internal", true ) ).first->second );
  
  result.version = 0;
  result.width = image_data.width();
  result.height = image_data.height();
  
  layer& layer( result.layers[ "Layer" ] );
  layer.index = 0;
  layer.x = 0;
  layer.y = 0;
  layer.width = result.width;
  layer.height = result.height;

  return true;
}

void pms::resources::image_mapping::load_with_gimp
( const std::string& name, const std::string& file_path )
{
  claw::logger << claw::log_verbose << "Using gimp…\n";
  
  std::istringstream info( execute_xcfinfo_process( file_path ) );

  image& result
    ( m_images.emplace
      ( std::piecewise_construct,
        std::forward_as_tuple( name ),
        std::forward_as_tuple( "XCF", false ) ).first->second );

  std::string line;
  
  std::getline( info, line );
  parse_xcf_info_header( result, line );

  while ( std::getline( info, line ) )
    parse_xcf_info_layer( result, line );
}

boost::optional< const pms::resources::image& >
pms::resources::image_mapping::get_image( const std::string& name ) const
{
  const auto it( m_images.find( name ) );

  if ( it != m_images.end() )
    return it->second;

  return boost::none;
}

std::string pms::resources::image_mapping::to_string() const
{
  std::ostringstream oss;
  oss << "Directory is '" << m_image_directory << "'\n";

  for ( const auto& e : m_images )
    oss << "- '" << e.first << "': " << e.second.to_string() << '\n';

  return oss.str();
}

std::string
pms::resources::image_mapping::execute_xcfinfo_process
( const std::string& file_path ) const
{
  gimp::system_interface::path_list_type includes;
  includes.push_back( "info.scm" );

  const std::string script( "(xcfinfo \"" + file_path + "\")" );
  std::istringstream iss( m_gimp.run( script, includes ) );

  std::string line;
  std::string result;
  const std::string prefix( "|- " );

  while( std::getline( iss, line ) )
    if ( line.find( prefix ) == 0 )
      result += line.substr( prefix.length() ) + '\n';

  return result;
}

void pms::resources::image_mapping::parse_xcf_info_header
( image& info, const std::string& header ) const
{
  std::istringstream iss( header );

  std::string version;

  iss >> version
      >> info.width
      >> info.height
    ; // ignoring everything after the size.

  if ( version.find( "2.6" ) == 0 )
    info.version = 2;
  else
    info.version = 3;

  if ( !iss )
    std::cerr << "Failed to read info header: '" << header
              << "'. Does the file exists?" << std::endl;
}

void pms::resources::image_mapping::parse_xcf_info_layer
( image& info, const std::string& layer_info ) const
{
  std::istringstream iss( layer_info );

  layer result;
  result.index = info.layers.size();

  iss >> result.width
      >> result.height
      >> result.x
      >> result.y
    ;

  /* skip the space that separates the offset and the layer name. */ 
  iss.ignore();

  if ( !iss )
    std::cerr << "Failed to read layer info: '" << layer_info << "'"
              << std::endl;

  std::string layer_name;
  std::getline( iss, layer_name );

  info.layers[ layer_name ] = result;
}
