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
#ifndef __SDC_XCF_INFO_HPP__
#define __SDC_XCF_INFO_HPP__

#include "layer_info.hpp"

#include <map>

namespace sdc
{
  class xcf_info
  {
  public:
    typedef std::map<std::string, layer_info> layer_map;

  public:
    std::string get_layer_name( std::size_t index ) const;

  public:
    int version;
    std::size_t width;
    std::size_t height;
    layer_map layers;

  };
}

#endif
