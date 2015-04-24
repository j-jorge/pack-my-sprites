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
#ifndef __PMS_SPRITE_SHEET_HPP__
#define __PMS_SPRITE_SHEET_HPP__

#include "spritedesc.hpp"
#include "xcf_map.hpp"

#include <list>

namespace pms
{
  class sprite_sheet
  {
  public:
    sprite_sheet();
    explicit sprite_sheet( xcf_map m );

    std::string to_string() const;

  public:
    spritedesc description;
    xcf_map xcf;

  };
}

#endif
