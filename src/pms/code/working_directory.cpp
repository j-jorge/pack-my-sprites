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
#include "working_directory.hpp"

#include <boost/filesystem/convenience.hpp>

bool pms::working_directory::is_fully_qualified( std::string filename )
{
  return !filename.empty() && ( filename[ 0 ] == '/' );
}

pms::working_directory::working_directory( std::string input_file_name )
  : m_input_file_name( input_file_name )
{
  const boost::filesystem::path input_path( m_input_file_name );
  const boost::filesystem::path input_directory( input_path.parent_path() );

  m_input_directory = input_directory.string();

  if ( m_input_directory.empty() )
    m_input_directory = '.';

  m_output_directory = m_input_directory;
}

std::string pms::working_directory::get_input_file_name() const
{
  return m_input_file_name;
}

std::string pms::working_directory::get_xcf_path( std::string xcf_name ) const
{
  if ( is_fully_qualified( xcf_name ) )
    return xcf_name;

  return m_input_directory + '/' + xcf_name;
}

std::string
pms::working_directory::get_output_image_path( std::string sheet_name ) const
{
  return m_output_directory + '/' + sheet_name + ".png";
}

std::string
pms::working_directory::get_output_file_path
( std::string sheet_name, std::string extension ) const
{
  return m_output_directory + '/' + sheet_name + "." + extension;
}
