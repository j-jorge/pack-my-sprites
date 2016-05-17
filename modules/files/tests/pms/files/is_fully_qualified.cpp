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
#include "pms/files/is_fully_qualified.hpp"

#include "utest/utest.hpp"

int main()
{
  TEST( pms::files::is_fully_qualified( "/abc" ) );
  TEST( pms::files::is_fully_qualified( "/abc/de" ) );
  TEST( pms::files::is_fully_qualified( "/abc/de/" ) );
  TEST( !pms::files::is_fully_qualified( "abc" ) );
  TEST( !pms::files::is_fully_qualified( "abc/de" ) );

  return 0;
}
