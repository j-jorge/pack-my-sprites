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

#include <boost/optional.hpp>

#include <iostream>

class program_arguments
{
public:
  pms::app::packer_options packer_options;
  std::vector< std::string > files;
};

class program_options_parser
{
public:
  program_options_parser();

  boost::optional< program_arguments > parse( int argc, char** argv );

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
      .add( pms::app::get_packer_program_options() ) )
{
  m_input_options.add_options()
    ( "input",
      boost::program_options::value< std::vector< std::string > >() );

  m_all_options.add( m_command_line_options ).add( m_input_options );
  m_positional_options.add( "input", -1 );
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
       ( m_command_line_options, arguments, argv[ 0 ], "[ options ] file…" ) )
    return boost::none;

  program_arguments result;

  result.packer_options = pms::app::parse_packer_program_options( arguments );

  if ( arguments.count( "input" ) == 0 )
    result.files.push_back( "-" );
  else
    result.files = arguments[ "input" ].as< std::vector< std::string > >();

  return result;
}

bool pack_from_command_line( int argc, char** argv )
{
  const boost::optional< program_arguments > arguments
    ( program_options_parser().parse( argc, argv ) );

  if ( !arguments )
    return true;

  pms::app::packer packer( arguments->packer_options );
  return packer.run( arguments->files );
}

int main( int argc, char** argv )
{
  try
    {
      if ( pack_from_command_line( argc, argv ) )
        return 0;

      std::cerr << "Could not build the sprite sheets.\n";
    }
  catch( const std::exception& e )
    {
      std::cerr << e.what() << '\n';
    }

  return 1;
}
