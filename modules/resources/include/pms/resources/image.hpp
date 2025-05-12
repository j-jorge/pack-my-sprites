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

#include "pms/resources/layer.hpp"

#include <claw/rectangle.hpp>
#include <claw/graphic/image.hpp>

#include <string>
#include <unordered_map>
#include <memory>

namespace pms
{
  namespace resources
  {
    class image
    {
    public:
      image( const std::string& format, bool internally_supported );

      std::string get_layer_name( std::size_t index ) const;
      std::string to_string() const;

    public:
      const std::string format;
      const bool internally_supported;
      int version;
      unsigned int width;
      unsigned int height;
      std::unordered_map< std::string, layer > layers;
      claw::math::rectangle< int > content_box;
      std::unique_ptr<const claw::graphic::image> bitmap;
    };
  }
}
