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
#include "spritepos_generator.hpp"

#include <fstream>

#include <claw/logger.hpp>

void pms::spritepos_generator::generate
( std::string spritedesc_file_path, sprite_sheet sheet ) const
{
  const working_directory dir( spritedesc_file_path );

  claw::logger << claw::log_verbose
               << "Generating spritepos file for sprite sheet '"
               << sheet.description.output_name << "' of '"
               << spritedesc_file_path << "'"
               << std::endl;

  generate_spritepos( dir, sheet.description );
}

void pms::spritepos_generator::generate_spritepos
( working_directory dir, spritedesc desc ) const
{
  const std::string filename
    ( dir.get_output_file_path( desc.output_name, "spritepos" ) );

  std::ofstream f( filename.c_str() );
  generate_spritepos( f, desc );
}

void pms::spritepos_generator::generate_spritepos
( std::ostream& os, spritedesc desc ) const
{
  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    {
      os << it->name << ": " << it->result_box.position.x << ' '
         << it->result_box.position.y << ' '
         << it->result_box.width << ' ' << it->result_box.height;

      if ( it->rotated )
        os << " r";

      os << '\n';
    }
}
