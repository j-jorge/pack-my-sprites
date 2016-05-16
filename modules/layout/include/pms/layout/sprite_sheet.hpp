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
#pragma once

#include "pms/layout/description.hpp"
#include "pms/resources/image_mapping.hpp"

namespace pms
{
  namespace layout
  {
    class sprite_sheet
    {
    public:
      sprite_sheet();
      explicit sprite_sheet( const resources::image_mapping& m );

      std::string to_string() const;

      bool internally_supported() const;
    
    public:
      layout::description description;
      resources::image_mapping image;
    };
  }
}