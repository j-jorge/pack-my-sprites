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
/**
 * \file
 * \brief Implementation of the sdc::xcf_map class.
 * \author Julien Jorge
 */
#include "xcf_map.hpp"

#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
sdc::xcf_map::xcf_map()
  : m_xcf_directory( "." )
{

} // xcf_map::xcf_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param xcf_directory The directory where the xcf files are taken.
 * \param gimp The interface to use to execute the GIMP scripts.
 */
sdc::xcf_map::xcf_map
( std::string xcf_directory, gimp_interface gimp )
  : m_xcf_directory( xcf_directory ), m_gimp( gimp )
{
  if ( m_xcf_directory.empty() )
    m_xcf_directory = '.';
} // xcf_map::xcf_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads the description of an xcf file.
 * \param name The name of the xcf file.
 */
void sdc::xcf_map::load( std::string name )
{
  if ( has_info(name) )
    return;

  std::istringstream info( execute_xcfinfo_process( name ) );

  xcf_info result;
  std::string line;
  
  std::getline( info, line );
  parse_xcf_info_header( result, line );

  while ( std::getline( info, line ) )
    parse_xcf_info_layer( result, line );

  m_xcf_info[ name ] = result;
} // xcf_map::load()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the map contains a given xcf file.
 * \param name The name of the xcf file.
 */
bool sdc::xcf_map::has_info( std::string name ) const
{
  return m_xcf_info.find( name ) != m_xcf_info.end();
} // xcf_map::has_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the informations of a given xcf file.
 * \param name The name of the xcf file.
 */
sdc::xcf_info sdc::xcf_map::get_info( std::string name ) const
{
  return m_xcf_info.find( name )->second;
} // xcf_map::get_info()

/*----------------------------------------------------------------------------*/
/**
 * \brief Executes the script that extracts the informations of a XCF file.
 * \param filename The name of the xcf file.
 */
std::string sdc::xcf_map::execute_xcfinfo_process( std::string filename ) const
{
  gimp_interface::path_list_type includes;
  includes.push_back( "info.scm" );

  const std::string script
    ( "(xcfinfo \"" + m_xcf_directory + '/' + filename + "\")" );
  std::istringstream iss( m_gimp.run( script, includes ) );

  std::string line;
  std::string result;
  const std::string prefix( "|- " );

  while( std::getline( iss, line ) )
    if ( line.find( prefix ) == 0 )
      result += line.substr( prefix.length() ) + '\n';

  return result;
} // xcf_map::execute_xcfinfo_process()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parses the first line of the output of the xcfinfo program in order to
 *        extract the required informations about the xcf file.
 * \param info The structure that receives the result of the parsing.
 * \param header The first line of the output of the xcfinfo program.
 */
void sdc::xcf_map::parse_xcf_info_header
( xcf_info& info, std::string header ) const
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
    std::cerr << "Failed to read info header: '" << header << "'" << std::endl;
} // xcf_map::parse_xcf_info_header()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parses a layer line of the output of the xcfinfo program in order to
 *        extract the required informations about the layer.
 * \param info The structure that receives the result of the parsing.
 * \param layer A layer line of the output of the xcfinfo program.
 */
void sdc::xcf_map::parse_xcf_info_layer
( xcf_info& info, std::string layer ) const
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
} // xcf_map::parse_xcf_info_layer()
