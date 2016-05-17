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

#include "utest/utest.hpp"

int main()
{
  pms::resources::image image( "test", true );
  
  TEST( image.format == "test" );
  TEST( image.internally_supported );

  image.layers[ "layer 1" ].index = 1;
  image.layers[ "layer 2" ].index = 4;

  TEST( image.get_layer_name( 1 ) == "layer 1" );
  TEST( image.get_layer_name( 4 ) == "layer 2" );
  TEST( image.get_layer_name( 0 ) == "" );
  
  return 0;
}
