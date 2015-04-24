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

#include <sstream>

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

std::string sdc::spritedesc::to_string() const
{
  std::ostringstream oss;

  oss << "output '" << output_name << "', size " << width << "×" << height
      << ", margin " << margin << ", placement in ";

  switch( order )
    {
    case decreasing_area:
      oss << "decreasing area";
      break;
    case decreasing_height:
      oss << "decreasing height";
      break;
    case declaration_order:
      oss << "declaration order";
      break;
    }

  if ( sprites.empty() )
    return oss.str();

  oss << ", sprites:\n";

  for ( sprite_list::const_iterator it( sprites.begin() );
        it != sprites.end(); ++it )
    {
      oss << "  * '" << it->name
          << "' with xcf '" << xcf.find( it->xcf_id )->second
          << "', source box (left=" << it->source_box.position.x
          << ", right=" << it->source_box.position.y
          << ", width=" << it->source_box.width
          << ", height=" << it->source_box.height
          << "), output box (left=" << it->result_box.position.x
          << ", right=" << it->result_box.position.y
          << ", width=" << it->result_box.width
          << ", height=" << it->result_box.height
          << "), layers [";

      for ( std::list<layer_info>::const_iterator layer( it->layers.begin() );
            layer != it->layers.end(); ++layer )
        oss << ' ' << layer->index;

      oss << " ], mask [";

      for ( std::list<layer_info>::const_iterator layer( it->mask.begin() );
            layer != it->mask.end(); ++layer )
        oss << ' ' << layer->index;
        
      oss << " ]\n";
    }

  return oss.str();
}
