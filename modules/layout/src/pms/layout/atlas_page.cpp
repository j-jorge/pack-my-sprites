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
#include "pms/layout/atlas_page.hpp"

#include <sstream>

pms::layout::atlas_page::const_sprite_iterator
pms::layout::atlas_page::get_sprite_by_name( const std::string& n ) const
{
  return
    std::find_if
    ( sprites.begin(), sprites.end(),
      [&]( const sprite& s ) -> bool
      {
        return s.name == n;
      } );
}

void pms::layout::atlas_page::add_sprite( const sprite& s )
{
  if ( (s.source_box.width <= 0) || (s.source_box.height <= 0) )
    std::cerr << "Sprite '" << s.name << "' is not visible. Skipping."
              << std::endl;
  else
    sprites.push_back( s );
}

pms::layout::atlas_page::const_sprite_iterator
pms::layout::atlas_page::sprite_begin() const
{
  return sprites.begin();
}

pms::layout::atlas_page::const_sprite_iterator
pms::layout::atlas_page::sprite_end() const
{
  return sprites.end();
}

pms::layout::atlas_page::sprite_iterator
pms::layout::atlas_page::sprite_begin()
{
  return sprites.begin();
}

pms::layout::atlas_page::sprite_iterator
pms::layout::atlas_page::sprite_end()
{
  return sprites.end();
}

std::size_t pms::layout::atlas_page::sprite_count() const
{
  return sprites.size();
}

void pms::layout::atlas_page::erase_sprite( const sprite_iterator& it )
{
  sprites.erase( it );
}

std::string pms::layout::atlas_page::to_string() const
{
  std::ostringstream oss;

  oss << "size " << width << "×" << height;

  if ( sprites.empty() )
    {
      oss << '\n';
      return oss.str();
    }

  oss << ", sprites:\n";

  for ( const sprite& s : sprites )
    {
      oss << "  * '" << s.name
          << "' with image '" << images.find( s.image_id )->second
          << "', source box (left=" << s.source_box.position.x
          << ", top=" << s.source_box.position.y
          << ", width=" << s.source_box.width
          << ", height=" << s.source_box.height
          << "), output box (left=" << s.result_box.position.x
          << ", right=" << s.result_box.position.y
          << ", width=" << s.result_box.width
          << ", height=" << s.result_box.height
          << ", rotate=" << s.rotated
          << ", bleed=" << s.bleed
          << "), layers [";

      for ( const resources::layer& layer : s.layers )
        oss << ' ' << layer.index;

      oss << " ], mask [";

      for ( const resources::layer& layer : s.mask )
        oss << ' ' << layer.index;
        
      oss << " ]\n";
    }

  return oss.str();
}
