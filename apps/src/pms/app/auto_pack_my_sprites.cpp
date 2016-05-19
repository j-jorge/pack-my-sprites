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
#include "pms/app/general_program_options.hpp"
#include "pms/app/packer.hpp"
#include "pms/app/packer_program_options.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <regex>

class dimensions
{
public:
  dimensions( std::uint32_t w, std::uint32_t h)
    : width( w ),
      height( h )
  { }
  
public:
  std::uint32_t width;
  std::uint32_t height;
};

void validate
( boost::any& v, const std::vector< std::string >& values, dimensions*, int)
{
  boost::program_options::validators::check_first_occurrence( v );
  const std::string& arg
    ( boost::program_options::validators::get_single_string( values ) );

  static std::regex format("(\\d+)x(\\d+)");
  std::smatch match;

  if ( std::regex_match( arg, match, format ) )
    v =
      boost::any
      ( dimensions
        ( boost::lexical_cast< std::uint32_t >( match[ 1 ] ),
          boost::lexical_cast< std::uint32_t >( match[ 2 ] ) ) );
  else
    throw boost::program_options::validation_error
      ( boost::program_options::validation_error::invalid_option_value );
}

class program_arguments
{
public:
  program_arguments();
  
public:
  pms::app::packer_options packer_options;
  std::string output;
  std::uint32_t margin;
  bool strip_paths;
  dimensions canvas_size;
  std::vector< std::string > files_bleeding;
  std::vector< std::string > files_dry;
};

program_arguments::program_arguments()
  : output( "sprite-sheet.png" ),
    margin( 1 ),
    canvas_size( 1024, 1024 )
{

}

class program_options_parser
{
public:
  program_options_parser();

  boost::optional< program_arguments > parse( int argc, char** argv );

private:
  static boost::program_options::options_description get_app_options();

private:
  boost::program_options::options_description m_command_line_options;
  boost::program_options::options_description m_input_options;
  boost::program_options::options_description m_all_options;

  boost::program_options::positional_options_description m_positional_options;
};

program_options_parser::program_options_parser()
  : m_command_line_options
    ( boost::program_options::options_description()
      .add( pms::app::get_general_program_options() )
      .add( pms::app::get_packer_program_options() )
      .add( get_app_options() ) )
{
  m_input_options.add_options()
    ( "dry",
      boost::program_options::value< std::vector< std::string > >() );

  m_all_options.add( m_command_line_options ).add( m_input_options );
  m_positional_options.add( "dry", -1 );
}

boost::program_options::options_description
program_options_parser::get_app_options()
{
  boost::program_options::options_description result( "Sprite sheet options" );

  static const program_arguments defaults;
  
  result.add_options()
    ( "bleeding",
      boost::program_options::value< std::vector< std::string > >()
      ->value_name( "file…" )
      ->multitoken(),
      "Enable the bleeding for the next file arguments" )
    ( "margin,m", boost::program_options::value< std::uint32_t >(),
      ( "The padding between the sprites. Default is "
      + boost::lexical_cast< std::string >( defaults.margin )
        + "." ).c_str() )
    ( "output,o", boost::program_options::value< std::string >(),
      ( "The name of the output file. Default is " + defaults.output
        + "." ).c_str() )
    ( "strip-paths,p",
      "Remove the directories from the input file names when naming the"
      " sprites." )
    ( "size,z",
      boost::program_options::value< dimensions >()->value_name( "WxH" ),
      ( "Define the maximum size of the resulting sprite sheet to a width of"
        " W pixels and a height of H pixels. Default size is "
        + boost::lexical_cast< std::string >( defaults.canvas_size.width ) + "x"
        + boost::lexical_cast< std::string >( defaults.canvas_size.height )
        + "." ).c_str() )
    ;
  return result;
}

boost::optional< program_arguments >
program_options_parser::parse( int argc, char** argv )
{
  boost::program_options::variables_map arguments;
  boost::program_options::store
    ( boost::program_options::command_line_parser( argc, argv )
      .options( m_all_options ).positional( m_positional_options ).run(),
      arguments );

  if ( !pms::app::parse_general_program_options
       ( m_command_line_options, arguments, argv[ 0 ],
         "[ options ] file… [ --bleeding file… ]" ) )
    return boost::none;

  program_arguments result;

  result.packer_options = pms::app::parse_packer_program_options( arguments );

  if ( arguments.count( "output" ) != 0 )
    result.output = arguments[ "output" ].as< std::string >();
  
  if ( arguments.count( "margin" ) != 0 )
    result.margin = arguments[ "margin" ].as< std::uint32_t >();
  
  result.strip_paths = ( arguments.count( "strip-paths" ) != 0 );
  
  if ( arguments.count( "size" ) != 0 )
    result.canvas_size = arguments[ "size" ].as< dimensions >();

  if ( arguments.count( "bleeding" ) != 0 )
    result.files_bleeding =
      arguments[ "bleeding" ].as< std::vector< std::string > >();

  if ( arguments.count( "dry" ) != 0 )
    result.files_dry = arguments[ "dry" ].as< std::vector< std::string > >();
  
  return result;
}

void configure_description
( pms::layout::description& result, const program_arguments& arguments )
{
  result.width = arguments.canvas_size.width;
  result.height = arguments.canvas_size.height;
  result.margin = arguments.margin;

  result.output_name = arguments.output;

  const std::string::size_type dot( result.output_name.find_last_of( '.' ) );

  if ( ( dot == std::string::npos )
       || ( dot != result.output_name.size() - 4 ) )
    return;

  result.output_name.erase
    ( result.output_name.begin() + dot, result.output_name.end() );
}

std::string stripped_name( const std::string& file )
{
  return boost::filesystem::path( file ).filename().string();
}

typedef std::unordered_map< std::string, std::string > file_to_name_mapping;

file_to_name_mapping
build_stripped_file_identifiers( const program_arguments& arguments )
{
  file_to_name_mapping result;
  result.reserve
    ( arguments.files_dry.size() + arguments.files_bleeding.size() );

  for ( const auto& file : arguments.files_bleeding )
    result[ file ] = stripped_name( file );

  for ( const auto& file : arguments.files_dry )
    result[ file ] = stripped_name( file );

  return result;
}

file_to_name_mapping
build_file_identifiers( const program_arguments& arguments )
{
  file_to_name_mapping result;
  result.reserve
    ( arguments.files_dry.size() + arguments.files_bleeding.size() );

  for ( const auto& file : arguments.files_bleeding )
    result[ file ] = file;

  for ( const auto& file : arguments.files_dry )
    result[ file ] = file;

  return result;
}

file_to_name_mapping identify_input_files( const program_arguments& arguments )
{
  if ( arguments.strip_paths )
    return build_stripped_file_identifiers( arguments );
  else
    return build_file_identifiers( arguments );
}

void set_description_files
( pms::layout::description& description, const file_to_name_mapping& names )
{
  for ( const auto& e : names )
    description.images[ e.second ] = e.first;
}

void load_images
( pms::resources::image_mapping& images,
  const std::vector< std::string >& files )
{
  for ( const std::string& s : files )
    images.load( s );
}

pms::layout::description::sprite create_sprite
( const std::string& name, const std::string& file,
  const pms::resources::image_mapping& images, bool bleeding )
{
  pms::layout::description::sprite result;

  result.image_id = result.name = name;
  result.bleed = bleeding;

  assert( images.get_image( file ) );
  const pms::resources::image& image( *images.get_image( file ) );

  assert( image.layers.size() == 1 );
  result.layers.push_back( image.layers.begin()->second );
  result.source_box.set( 0, 0, image.width, image.height );
  result.result_box = result.source_box;

  return result;
}

pms::layout::sprite_sheet
build_sprite_sheet( const program_arguments& arguments )
{
  pms::layout::description description;

  configure_description( description, arguments );
  const file_to_name_mapping file_to_name( identify_input_files( arguments ) );
  set_description_files( description, file_to_name );
  
  pms::resources::image_mapping images
    ( ".",
      pms::gimp::system_interface
      ( arguments.packer_options.scheme_directory,
        arguments.packer_options.gimp_console_program ) );

  load_images( images, arguments.files_dry );
  load_images( images, arguments.files_bleeding );
  
  for ( const std::string& file : arguments.files_dry )
    description.add_sprite
      ( create_sprite
        ( file_to_name.find( file )->second, file, images, false ) );
  
  for ( const std::string& file : arguments.files_bleeding )
    description.add_sprite
      ( create_sprite
        ( file_to_name.find( file )->second, file, images, true ) );

  pms::layout::sprite_sheet result( images );
  result.description = description;
  
  return result;
}

bool pack_from_command_line( int argc, char** argv )
{
  const boost::optional< program_arguments > arguments
    ( program_options_parser().parse( argc, argv ) );

  if ( !arguments )
    return false;

  pms::app::packer packer( arguments->packer_options );
  return packer.run( "memory", build_sprite_sheet( *arguments ) );
}

int main( int argc, char** argv )
{
  try
    {
      if ( pack_from_command_line( argc, argv ) )
        return 0;
    }
  catch( const std::exception& e )
    {
      std::cerr << e.what() << '\n';
    }

  return 1;
}
