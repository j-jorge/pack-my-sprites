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
#include "pms/resources/image.hpp"

#include <sstream>

pms::resources::image::image
( const std::string& format, bool internally_supported )
  : format( format ),
    internally_supported( internally_supported )
{

}

std::string pms::resources::image::get_layer_name( std::size_t index ) const
{
  for( const auto& e : layers )
    if ( e.second.index == index )
      return e.first;

  return std::string();
}

std::string pms::resources::image::to_string() const
{
  std::ostringstream oss;
  oss << "version " << version << " size " << width << "×" << height;

  if ( layers.empty() )
    return oss.str();

  oss << " layers:\n";

  for( const auto& e : layers )
    oss << "  [" << e.second.index << "] '" << e.first
        << "' left=" << e.second.x
        << ", top=" << e.second.y
        << ", width=" << e.second.width
        << ", height=" << e.second.height
        << '\n';

  return oss.str();
}
