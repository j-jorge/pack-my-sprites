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
#include "pms/app/packer_program_options.hpp"

#include "pms/app/atlas_format.hpp"

#include <boost/preprocessor/stringize.hpp>

namespace pms
{
  namespace app
  {
    static void validate
    ( boost::any& v, const std::vector< std::string >& values,
      atlas_format*, int );
  }
}

boost::program_options::options_description
pms::app::get_packer_program_options()
{
  boost::program_options::options_description result( "Packer options" );

  result.add_options()
    ( "scheme-directory,s",
      boost::program_options::value< std::vector< std::string > >()
      ->value_name( "directory" ),
      "A directory where the utility Scheme scripts can be found." )
    ( "gimp-console,g",
      boost::program_options::value< std::string >()->value_name( "file" ),
      "The path to the gimp-console executable." )
    ( "no-crop,c",
      "Disable cropping the sprites in the sprite sheet." )
    ( "no-rotation,r",
      "Disable the rotation of the sprites in the sprite sheet." )
    ( "format,f", boost::program_options::value< atlas_format >(),
      "The format of the atlas to generate. Valid values are:\n\n"
      "css\n"
      "  \tGenerate a CSS file for the sprite sheet.\n\n"
      "plist\n"
      "  \tGenerate a Zwoptex-like PList file for the sprite sheet.\n\n"
      "spine\n"
      "  \tGenerate a Spine-like atlas file for the sprite sheet.\n\n"
      "spritepos\n"
      "  \tGenerate a spritepos file for the sprite sheet. Implies"
      " --no-rotation." )
    ;

  return result;
}

pms::app::packer_options
pms::app::parse_packer_program_options
( const boost::program_options::variables_map& values )
{
  packer_options result;

  result.set_atlas_format( values[ "format" ].as< atlas_format >() );

  if ( values.count( "gimp-console" ) != 0 )
    result.set_gimp_console_program
      ( values[ "gimp-console" ].as< std::string >() );

  if ( values.count( "no-rotation" ) != 0 )
    result.disable_rotation();

  if ( values.count( "no-crop" ) != 0 )
    result.disable_crop();

  if ( values.count( "scheme-directory" ) != 0 )
      result.add_scheme_directories
        ( values[ "scheme-directory" ].as< std::vector< std::string > >() );
  
#ifdef PMS_DEFAULT_SCHEME_PATH
  result.add_scheme_directories
    ( { BOOST_PP_STRINGIZE( PMS_DEFAULT_SCHEME_PATH ) } );
#endif
  
  return result;
}

void pms::app::validate
( boost::any& v, std::vector<std::string> const& values,
  atlas_format*, int )
{
  boost::program_options::validators::check_first_occurrence( v );

  const std::string& s
    ( boost::program_options::validators::get_single_string( values ) );

  if ( s == "spine" )
    v = boost::any( atlas_format::spine );
  else if ( s == "plist" )
    v = boost::any( atlas_format::plist );
  else if ( s == "spritepos" )
    v = boost::any( atlas_format::spritepos );
  else if ( s == "css" )
    v = boost::any( atlas_format::css );
  else
    throw boost::program_options::validation_error
      ( boost::program_options::validation_error::invalid_option_value );
}

