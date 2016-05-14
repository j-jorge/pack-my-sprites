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
#include "pms/generators/detail/working_directory.hpp"

#include "pms/files/is_fully_qualified.hpp"

#include <boost/filesystem/convenience.hpp>

pms::generators::detail::working_directory::working_directory
( const std::string& input_file_name )
  : m_input_file_name( input_file_name )
{
  const boost::filesystem::path input_path( m_input_file_name );
  const boost::filesystem::path input_directory( input_path.parent_path() );

  m_directory = input_directory.string();

  if ( m_directory.empty() )
    m_directory = '.';
}

const std::string&
pms::generators::detail::working_directory::get_input_file_name() const
{
  return m_input_file_name;
}

std::string
pms::generators::detail::working_directory::get_image_path
( const std::string& image_name ) const
{
  if ( files::is_fully_qualified( image_name ) )
    return image_name;

  return m_directory + '/' + image_name;
}

std::string
pms::generators::detail::working_directory::get_output_image_path
( const std::string& sheet_name ) const
{
  return m_directory + '/' + sheet_name + ".png";
}

std::string
pms::generators::detail::working_directory::get_output_file_path
( const std::string& sheet_name, const std::string& extension ) const
{
  return m_directory + '/' + sheet_name + "." + extension;
}
