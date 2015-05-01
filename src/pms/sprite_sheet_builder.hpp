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
#ifndef __PMS_SPRITE_SHEET_BUILDER_HPP__
#define __PMS_SPRITE_SHEET_BUILDER_HPP__

#include "sprite_sheet.hpp"

namespace pms
{
  class sprite_sheet_builder
  {
  public:
    sprite_sheet
    build( bool allow_rotate, sprite_sheet sprite_description ) const;

  private:
    void set_sprite_position( bool allow_rotate, spritedesc& desc ) const;
    spritedesc::sprite_iterator
    find_sprite_by_size( spritedesc& desc, int width, int height ) const;

  };
}

#endif
