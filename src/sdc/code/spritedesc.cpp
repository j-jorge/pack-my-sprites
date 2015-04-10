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
#include "spritedesc.hpp"

sdc::spritedesc::const_sprite_iterator
sdc::spritedesc::get_sprite_by_name( std::string n ) const
{
  for ( sprite_list::const_iterator it=sprites.begin(); it!=sprites.end();
        ++it )
    if ( it->name == n )
      return it;

  return sprites.end();
}

void sdc::spritedesc::add_sprite( sprite s )
{
  if ( (s.source_box.width <= 0) || (s.source_box.height <= 0) )
    std::cerr << "Sprite '" << s.name << "' is not visible. Skipping."
              << std::endl;
  else
    sprites.push_back( s );
}

sdc::spritedesc::const_sprite_iterator sdc::spritedesc::sprite_begin() const
{
  return sprites.begin();
}

sdc::spritedesc::const_sprite_iterator sdc::spritedesc::sprite_end() const
{
  return sprites.end();
}

sdc::spritedesc::sprite_iterator sdc::spritedesc::sprite_begin()
{
  return sprites.begin();
}

sdc::spritedesc::sprite_iterator sdc::spritedesc::sprite_end()
{
  return sprites.end();
}

void sdc::spritedesc::erase_sprite( sprite_iterator it )
{
  sprites.erase( it );
}
