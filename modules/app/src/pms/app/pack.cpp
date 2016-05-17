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
#include "pms/app/pack.h"

#include "pms/layout/build.hpp"

void pms::app::pack
( const packer_options& options, const std::string& source_file_path,
  layout::sprite_sheet desc ) const
{
  if ( !layout::build( options.enable_sprite_rotation, desc.description ) )
    {
      std::cerr << "Failed to build sprite sheet '" << source_file_path
                << "'.\n";
      return;
    }
      
  generators::png generator
    ( gimp::system_interface
      ( options.scheme_directory, options.gimp_console_program ) );
  generator.generate( source_file_path, desc );

  if ( options.generate_spritepos )
    {
      generators::spritepos generator;
      generator.generate( source_file_path, desc );
    }

  if ( options.generate_plist )
    {
      generators::plist generator;
      generator.generate( source_file_path, desc );
    }

  if ( options.generate_css )
    {
      generators::css generator;
      generator.generate( source_file_path, desc );
    }
}

