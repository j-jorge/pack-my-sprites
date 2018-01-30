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
#include "pms/app/packer_options.hpp"

#include "pms/app/atlas_format.hpp"
#include "pms/generators/color_mode.hpp"
#include "pms/generators/rotation_direction.hpp"

pms::app::packer_options::packer_options()
  : m_atlas_format( atlas_format::none ),
    m_allow_rotation( true ),
    m_allow_crop( true ),
    m_premultiply_alpha( true ),
    m_gimp_console_program( "gimp-console" )
{

}

pms::app::atlas_format pms::app::packer_options::get_atlas_format() const
{
  return m_atlas_format;
}

void pms::app::packer_options::set_atlas_format( atlas_format t )
{
  m_atlas_format = t;
}
      
pms::generators::rotation_direction
pms::app::packer_options::get_rotation_direction() const
{
  if ( !m_allow_rotation )
    return generators::rotation_direction::none;

  switch( m_atlas_format )
    {
    case atlas_format::css:
    case atlas_format::plist:
      return generators::rotation_direction::clockwise;
    case atlas_format::spine:
      return generators::rotation_direction::anticlockwise;
    default:
      return generators::rotation_direction::none;
    }
}

void pms::app::packer_options::disable_rotation()
{
  m_allow_rotation = false;
}
      
pms::generators::color_mode
pms::app::packer_options::get_color_mode() const
{
  if ( m_premultiply_alpha )
    return generators::color_mode::multiply_alpha;
  
  return generators::color_mode::copy;
}

void pms::app::packer_options::disable_premultiplied_alpha()
{
  m_premultiply_alpha = false;
}
      
bool pms::app::packer_options::should_crop() const
{
  if ( !m_allow_crop )
    return false;

  switch( m_atlas_format )
    {
    case atlas_format::plist:
    case atlas_format::spine:
      return true;
    default:
      return false;
    }
}

void pms::app::packer_options::disable_crop()
{
  m_allow_crop = false;
}
      
const std::vector< std::string >&
pms::app::packer_options::get_scheme_directory() const
{
  return m_scheme_directory;
}

void pms::app::packer_options::add_scheme_directories
( const std::vector< std::string >& dirs )
{
  m_scheme_directory.insert
    ( m_scheme_directory.end(), dirs.begin(), dirs.end() );
}

const std::string& pms::app::packer_options::get_gimp_console_program() const
{
  return m_gimp_console_program;
}
  
void
pms::app::packer_options::set_gimp_console_program( const std::string& gimp )
{
  m_gimp_console_program = gimp;
}

