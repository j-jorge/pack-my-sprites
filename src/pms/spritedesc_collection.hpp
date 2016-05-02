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
#ifndef __PMS_SPRITEDESC_COLLECTION_HPP__
#define __PMS_SPRITEDESC_COLLECTION_HPP__

#include "image_map.hpp"
#include "spritedesc.hpp"

#include <list>

namespace pms
{
  class sprite_sheet
  {
  public:
    spritedesc_collection();
    explicit spritedesc_collection( const image_map& m );

  public:
    spritedesc sprite_sheet;
    image_map image;

  };
}

#endif
