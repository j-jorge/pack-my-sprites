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
#include "pms/serialization/read_spritedesc.hpp"

#include "pms/serialization/spritedesc/parser.hpp"

#include "pms/layout/atlas.hpp"

pms::layout::atlas
pms::serialization::read_spritedesc
( const resources::image_mapping& images, std::istream& is )
{
  layout::atlas result( images );
  
  spritedesc::parser p;
  p.run( result, is );

  return result;
}
