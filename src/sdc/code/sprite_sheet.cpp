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
#include "sprite_sheet.hpp"

#include <sstream>

sdc::sprite_sheet::sprite_sheet()
{

}

sdc::sprite_sheet::sprite_sheet( xcf_map m )
  : xcf( m )
{

}

std::string sdc::sprite_sheet::to_string() const
{
  std::ostringstream oss;
  oss << "XCF:\n" << xcf.to_string() << "Description:\n"
      << description.to_string() << '\n';

  return oss.str();
}
