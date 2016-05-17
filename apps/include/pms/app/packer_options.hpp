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
#pragma once

#include <vector>
#include <string>

namespace pms
{
  namespace app
  {
    class packer_options
    {
    public:
      packer_options();

    public:
      bool generate_spritepos;
      bool generate_plist;
      bool generate_css;
      std::vector< std::string > scheme_directory;
      std::string gimp_console_program;
      bool enable_sprite_rotation;
    };
  };
}
