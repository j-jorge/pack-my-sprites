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
#include "xcf_map.hpp"

#include "working_directory.hpp"

#include <claw/exception.hpp>
#include <claw/image.hpp>
#include <claw/logger.hpp>

#include <fstream>
#include <sstream>

pms::xcf_map::xcf_map()
  : m_xcf_directory( "." )
{

}

pms::xcf_map::xcf_map
( const std::string& xcf_directory, const gimp_interface& gimp )
  : m_xcf_directory( xcf_directory ), m_gimp( gimp )
{
  if ( m_xcf_directory.empty() )
    m_xcf_directory = '.';
}

void pms::xcf_map::load( const std::string& name )
{
  if ( has_info(name) )
    return;

  std::string file_path;

  if ( working_directory::is_fully_qualified( name ) )
    file_path = name;
  else
    file_path = m_xcf_directory + '/' + name;

  claw::logger << claw::log_verbose << "Loading '" << name << "' from '"
               << file_path << "'…" << std::endl;

  if ( load_with_internal_tool( name, file_path ) )
    return;

  load_with_gimp( name, file_path );
}

bool pms::xcf_map::load_with_internal_tool
( const std::string& name, const std::string& file_path )
{
  claw::logger << claw::log_verbose << "Using internal tool…\n";

  std::ifstream f( file_path );

  if ( !f )
    return false;
  
  claw::graphic::image image;

  try
    {
      claw::graphic::image( f ).swap( image );
    }
  catch( const claw::bad_format& f )
    {
      return false;
    }

  layer_info layer;
  layer.box.position.x = 0;
  layer.box.position.y = 0;
  layer.box.width = image.width();
  layer.box.height = image.height();
  
  xcf_info result;
  result.version = 0;
  result.width = layer.box.width;
  result.height = layer.box.height;

  result.layers[ "Image" ] = layer;

  m_xcf_info[ name ] = result;

  return true;
}

void pms::xcf_map::load_with_gimp
( const std::string& name, const std::string& file_path )
{
  claw::logger << claw::log_verbose << "Using gimp…\n";
  
  std::istringstream info( execute_xcfinfo_process( file_path ) );

  xcf_info result;
  std::string line;
  
  std::getline( info, line );
  parse_xcf_info_header( result, line );

  while ( std::getline( info, line ) )
    parse_xcf_info_layer( result, line );

  m_xcf_info[ name ] = result;
}

bool pms::xcf_map::has_info( const std::string& name ) const
{
  return m_xcf_info.find( name ) != m_xcf_info.end();
}

pms::xcf_info pms::xcf_map::get_info( const std::string& name ) const
{
  return m_xcf_info.find( name )->second;
}

std::string pms::xcf_map::to_string() const
{
  std::ostringstream oss;
  oss << "Directory is '" << m_xcf_directory << "'\n";

  for ( name_to_info_type::const_iterator it( m_xcf_info.begin() );
        it != m_xcf_info.end(); ++it )
    oss << "- '" << it->first << "': " << it->second.to_string() << '\n';

  return oss.str();
}

std::string
pms::xcf_map::execute_xcfinfo_process( const std::string& file_path ) const
{
  gimp_interface::path_list_type includes;
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

void pms::xcf_map::parse_xcf_info_header
( xcf_info& info, const std::string& header ) const
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

void pms::xcf_map::parse_xcf_info_layer
( xcf_info& info, const std::string& layer ) const
{
  std::istringstream iss( layer );

  layer_info result;
  result.index = info.layers.size();

  iss >> result.box.width
      >> result.box.height
      >> result.box.position.x
      >> result.box.position.y
    ;

  /* skip the space that separates the offset and the layer name. */ 
  iss.ignore();

  if ( !iss )
    std::cerr << "Failed to read layer info: '" << layer << "'" << std::endl;

  std::string layer_name;
  std::getline( iss, layer_name );

  info.layers[ layer_name ] = result;
}
