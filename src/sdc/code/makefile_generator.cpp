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
#include "makefile_generator.hpp"

#include <fstream>

#include <boost/algorithm/string/join.hpp>

sdc::makefile_generator::makefile_generator
( std::string makefile, std::string command )
  : m_makefile(makefile), m_command( command )
{

}

void sdc::makefile_generator::run( file_to_spritedesc_map file )
{
  std::ostream* output;
  bool output_to_file(false);

  if ( m_makefile == "-" )
    output = &std::cout;
  else
    {
      output = new std::ofstream( m_makefile.c_str() );
      output_to_file = true;
    }

  const std::vector<std::string> dependencies( get_all_output_files( file ) );

  *output << "all: " << boost::algorithm::join( dependencies, " " )
          << "\n\n";

  generate_makefile_rules( *output, file );

  if ( output_to_file )
    delete output;
}

std::vector<std::string> sdc::makefile_generator::get_all_output_files
( file_to_spritedesc_map file ) const
{
  std::vector<std::string> result;

  for ( file_to_spritedesc_map::const_iterator it=file.begin();
        it!=file.end(); ++it )
    {
      const std::vector<std::string> desc_files
        ( get_all_output_files( working_directory( it->first ), it->second ) );
      result.insert( result.end(), desc_files.begin(), desc_files.end() );
    }

  return result;
}

std::vector<std::string> sdc::makefile_generator::get_all_output_files
( working_directory dir, spritedesc_collection desc ) const
{
  std::vector<std::string> result;
  typedef
    spritedesc_collection::spritedesc_list_type::const_iterator iterator_type;

  for ( iterator_type it = desc.sprite_sheet.begin();
        it != desc.sprite_sheet.end();
        ++it )
    result.push_back( dir.get_output_image_path( it->output_name ) );

  return result;
}

void sdc::makefile_generator::generate_makefile_rules
( std::ostream& output, file_to_spritedesc_map file ) const
{
  for ( file_to_spritedesc_map::const_iterator it=file.begin();
        it!=file.end(); ++it )
    generate_makefile_rule
      ( output, working_directory(it->first), it->second.sprite_sheet );
}

void sdc::makefile_generator::generate_makefile_rule
( std::ostream& output, working_directory dir,
  std::list<spritedesc> desc ) const
{
  for ( std::list<spritedesc>::const_iterator it=desc.begin(); it!=desc.end();
        ++it )
    generate_makefile_rule( output, dir, *it );
}

void sdc::makefile_generator::generate_makefile_rule
( std::ostream& output, working_directory dir, spritedesc desc ) const
{
  output << dir.get_output_image_path( desc.output_name ) << ':';

  std::vector<std::string> xcf_files;

  for ( spritedesc::id_to_file_map::const_iterator xcf_it = desc.xcf.begin();
        xcf_it != desc.xcf.end(); ++xcf_it )
    output << ' ' <<  dir.get_xcf_path( xcf_it->second );

  output << ' ' << dir.get_input_file_name() << "\n";
  output << "\t" << m_command << " --target=\"" << desc.output_name << "\" "
         << dir.get_input_file_name() << "\n\n";
}
