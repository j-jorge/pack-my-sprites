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
#ifndef __PMS_SPRITEPOS_GENERATOR_HPP__
#define __PMS_SPRITEPOS_GENERATOR_HPP__

#include "sprite_sheet.hpp"
#include "working_directory.hpp"

namespace pms
{
  class spritepos_generator
  {
  public:
    void generate
    ( std::string spritedesc_file_path, sprite_sheet sheet ) const;

  private:
    void generate_spritepos( working_directory dir, spritedesc desc ) const;
    void generate_spritepos( std::ostream& os, spritedesc desc ) const;

  };
}

#endif
