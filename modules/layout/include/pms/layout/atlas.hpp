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

#include "pms/layout/atlas_page.hpp"
#include "pms/resources/image_mapping.hpp"

namespace pms
{
  namespace layout
  {
    class atlas
    {
    public:
      atlas();
      explicit atlas( const resources::image_mapping& m );

      std::string to_string() const;

      bool internally_supported() const;
      
    public:
      std::string output_name;
      std::size_t margin;
      std::size_t width;
      std::size_t height;

      std::vector< layout::atlas_page > pages;
      resources::image_mapping image;
    };
  }
}
