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
#include "pms/layout/description.hpp"

#include "utest/utest.hpp"

void get_sprite_by_name()
{
  pms::layout::description::sprite sprite;
  sprite.image_id = "a";
  sprite.name = "b";
  sprite.source_box.width = 10;
  sprite.source_box.height = 20;
  
  pms::layout::description description;
  description.add_sprite( sprite );

  const auto it( description.get_sprite_by_name( "b" ) );
  
  TEST( it != description.sprite_end() );

  TEST( it->image_id == sprite.image_id );
  TEST( it->name == sprite.name );
  TEST( it->source_box == sprite.source_box );
  TEST( it->result_box == sprite.result_box );
  TEST( it->rotated == sprite.rotated );
  TEST( it->bleed == sprite.bleed );
}

void skip_empty()
{
  pms::layout::description description;

  pms::layout::description::sprite no_width;
  no_width.name = "a";
  no_width.source_box.width = 0;
  
  description.add_sprite( no_width );

  pms::layout::description::sprite no_height;
  no_height.name = "b";
  no_height.source_box.height = 0;

  description.add_sprite( no_height );

  TEST( description.get_sprite_by_name( "a" ) == description.sprite_end() );
  TEST( description.get_sprite_by_name( "b" ) == description.sprite_end() );
}
    
void sprite_count()
{
  pms::layout::description::sprite sprite;
  sprite.image_id = "a";
  sprite.name = "b";
  sprite.source_box.width = 10;
  sprite.source_box.height = 20;
  
  pms::layout::description description;
  description.add_sprite( sprite );

  sprite.image_id = "2";
  sprite.name = "b";

  description.add_sprite( sprite );
  
  TEST( description.sprite_count() == 2 );
}

void erase_sprite()
{
  pms::layout::description::sprite sprite;
  sprite.image_id = "a";
  sprite.name = "b";
  sprite.source_box.width = 10;
  sprite.source_box.height = 20;
  
  pms::layout::description description;
  description.add_sprite( sprite );

  TEST( description.sprite_count() == 1 );

  description.erase_sprite( description.sprite_begin() );
  
  TEST( description.sprite_count() == 0 );
}

int main()
{
  get_sprite_by_name();
  skip_empty();
  sprite_count();
  erase_sprite();
  
  return 0;
}
