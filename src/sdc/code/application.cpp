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
#include "application.hpp"

#include <fstream>
#include <iostream>

#include <claw/logger.hpp>

#include "gimp_interface.hpp"
#include "image_generator.hpp"
#include "parser.hpp"
#include "plist_generator.hpp"
#include "sprite_sheet_builder.hpp"
#include "spritepos_generator.hpp"
#include "version.hpp"

#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string/join.hpp>

sdc::application::application( int& argc, char** &argv )
  : claw::application(argc, argv), m_quit(false), m_generate_spritepos(true),
    m_generate_plist(false),
    m_gimp_console_program( "gimp-console" )
{
  check_arguments( argc, argv );
}

int sdc::application::run()
{
  if (!m_quit)
    {
#ifdef SDC_DEFAULT_SCHEME_PATH
      m_scheme_directory.push_back( SDC_TO_STR( SDC_DEFAULT_SCHEME_PATH ) );
#endif

      process_files();
    }

  return 0;
}

void sdc::application::help() const
{
  m_arguments.help("file.spritedesc...");
}

void sdc::application::check_arguments( int& argc, char** &argv )
{
  m_arguments.add( "-h", "--help", "Print this message and exit.", true );
  m_arguments.add
    ( "-s", "--scheme-directory",
      "A directory where the utility Scheme scripts can be found.", true );
  m_arguments.add
    ( "-g", "--gimp-console",
      "The path to the gimp-console executable.", true );
  m_arguments.add_long
    ( "--no-spritepos", "Tells to not generate the spritepos file.", true );
  m_arguments.add_long
    ( "--plist", "Tells to not generate the PList file.", true );
  m_arguments.add_long
    ( "--version", "Prints the version of the software.", true );

  m_arguments.parse( argc, argv );

  if ( m_arguments.get_bool("--version") )
    {
      std::cout << SDC_VERSION_STRING << std::endl;
      m_quit = true;
    }

  if ( !m_quit && m_arguments.get_bool("--help") )
    {
      help();
      m_quit = true;
    }

  if ( m_arguments.has_value("--gimp-console") )
    m_gimp_console_program = m_arguments.get_string("--gimp-console");

  if ( m_arguments.has_value("--scheme-directory") )
    m_scheme_directory = m_arguments.get_all_of_string("--scheme-directory");

  m_generate_spritepos = !m_arguments.get_bool("--no-spritepos");
  m_generate_plist = m_arguments.get_bool("--plist");

  for ( int argi=0; argi!=argc; ++argi )
    m_input_file.push_back( argv[argi] );
}

void sdc::application::process_files()
{
  file_to_spritedesc_map content;

  if ( m_input_file.empty() )
    content[ "stdin" ] = read_spritedesc_stdin();
  else
    for ( std::size_t i=0; i!=m_input_file.size(); ++i )
      content[ m_input_file[i] ] = read_spritedesc_file( m_input_file[i] );

  generate_sprite_sheet_files( content );
}

sdc::sprite_sheet sdc::application::read_spritedesc_stdin() const
{
  return read_spritedesc_file( ".", std::cin );
}

sdc::sprite_sheet
sdc::application::read_spritedesc_file( std::string file_name ) const
{
  const boost::filesystem::path file_path( file_name );
  std::ifstream f( file_path.string().c_str() );

  if ( !f )
    {
      std::cerr << "Can't find file '" << file_name << "'." << std::endl;
      return sprite_sheet();
    }

  const boost::filesystem::path file_directory( file_path.parent_path() );

  return read_spritedesc_file( file_directory.string(), f );
}

sdc::sprite_sheet
sdc::application::read_spritedesc_file
( std::string directory, std::istream& in ) const
{
  const xcf_map xcf
    ( directory, gimp_interface( m_scheme_directory, m_gimp_console_program ) );
  sprite_sheet result( xcf );

  parser p;

  p.run( result.xcf, result.description, in );

  dump_sprite_sheet( result );

  return result;
}

void sdc::application::dump_sprite_sheet( sprite_sheet sheet ) const
{
  claw::logger << claw::log_verbose
               << "Sprite sheet dump:\n"
               << sheet.to_string();
}

void sdc::application::generate_sprite_sheet_files
( file_to_spritedesc_map sprite_sheet_description ) const
{
  typedef file_to_spritedesc_map::const_iterator iterator_type;

  for ( iterator_type it = sprite_sheet_description.begin();
        it != sprite_sheet_description.end(); ++it )
    {
      claw::logger << claw::log_verbose
                   << "Generating from file '" << it->first << "'"
                   << std::endl;
      generate_sprite_sheet_files( it->first, it->second );
    }
}

void sdc::application::generate_sprite_sheet_files
( std::string source_file_path, sprite_sheet desc ) const
{
  sprite_sheet_builder builder;
  desc = builder.build( desc );
      
  image_generator generator
    ( gimp_interface( m_scheme_directory, m_gimp_console_program ) );
  generator.generate( source_file_path, desc );

  if ( m_generate_spritepos )
    {
      spritepos_generator spritepos;
      spritepos.generate( source_file_path, desc );
    }

  if ( m_generate_plist )
    {
      plist_generator plist;
      plist.generate( source_file_path, desc );
    }
}

std::string sdc::application::get_self_command() const
{
  std::ostringstream result;

  result << m_arguments.get_program_name() << ' '
         << "--gimp-console=" << m_gimp_console_program;

  if ( !m_generate_spritepos )
    result << " --no-spritepos";

  if ( m_generate_plist )
    result << " --plist";

  for ( path_list_type::const_iterator it = m_scheme_directory.begin();
        it != m_scheme_directory.end(); ++it )
    result << " --scheme-directory=" << *it;

  return result.str();
}
