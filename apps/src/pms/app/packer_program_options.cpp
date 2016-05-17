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

#include <boost/preprocessor/stringize.hpp>

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
    ( "no-rotation,r",
      "Disable the rotation of the sprites in the sprite sheet." )
    ( "css", "Generate a CSS file for the sprite sheet." )
    ( "plist", "Generate a Zwoptex-like PList file for the sprite sheet." )
    ( "spritepos", "Generate a spritepos file for the sprite sheet." )
    ;

  return result;
}

pms::app::packer_options
pms::app::parse_packer_program_options
( const boost::program_options::variables_map& values )
{
  packer_options result;

  result.generate_spritepos = ( values.count( "spritepos" ) != 0 );
  result.generate_plist = ( values.count( "plist" ) != 0 );
  result.generate_css = ( values.count( "css" ) != 0 );

  if ( values.count( "gimp-console" ) != 0 )
    result.gimp_console_program = values[ "gimp-console" ].as< std::string >();

  result.enable_sprite_rotation = ( values.count( "no-rotation" ) == 0 );

  if ( values.count( "scheme-directory" ) != 0 )
      result.scheme_directory =
        values[ "scheme-directory" ].as< std::vector< std::string > >();
  
#ifdef PMS_DEFAULT_SCHEME_PATH
  result.scheme_directory.push_back
    ( BOOST_PP_STRINGIZE( PMS_DEFAULT_SCHEME_PATH ) );
#endif
  
  return result;
}

