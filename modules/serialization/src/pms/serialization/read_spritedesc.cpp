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
#include "pms/serialization/read_spritedesc.hpp"

#include "pms/serialization/spritedesc/parser.hpp"

#include "pms/layout/sprite_sheet.hpp"

pms::layout::sprite_sheet
pms::serialization::read_spritedesc
( const resources::image_mapping& images, std::istream& is )
{
  layout::sprite_sheet result( images );
  
  spritedesc::parser p;
  p.run( result, is );

  return result;
}
