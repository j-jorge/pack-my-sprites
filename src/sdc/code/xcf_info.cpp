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
#include "xcf_info.hpp"

#include <sstream>

std::string sdc::xcf_info::get_layer_name( std::size_t index ) const
{
  for( layer_map::const_iterator it=layers.begin(); it!=layers.end(); ++it )
    if ( it->second.index == index )
      return it->first;

  return std::string();
}

std::string sdc::xcf_info::to_string() const
{
  std::ostringstream oss;
  oss << "version " << version << " size " << width << "×" << height;

  if ( layers.empty() )
    return oss.str();

  oss << " layers:\n";

  for ( layer_map::const_iterator it( layers.begin() );
        it != layers.end(); ++it )
    oss << "  [" << it->second.index << "] '" << it->first
        << "' left=" << it->second.box.position.x
        << ", top=" << it->second.box.position.y
        << ", width=" << it->second.box.width
        << ", height=" << it->second.box.height
        << '\n';

  return oss.str();
}
