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
#include "pms/layout/atlas.hpp"

#include <sstream>
#include <unordered_set>

pms::layout::atlas::atlas()
{

}

pms::layout::atlas::atlas( const resources::image_mapping& m )
  : image( m )
{

}

std::string pms::layout::atlas::to_string() const
{
  std::ostringstream oss;
  oss << "Image:\n" << image.to_string();

  oss << "size: " << width << "×" << height << ", margin: " << margin <<'\n';
  
  const std::size_t page_count( pages.size() );
  
  for ( std::size_t i( 0 ); i != page_count; ++i )
    oss << "Page" << ( i + 1 ) << '/' << page_count << ":\n"
        << pages[ i ].to_string() << '\n';

  return oss.str();
}

bool pms::layout::atlas::internally_supported() const
{
  std::unordered_set< std::string > image_names;

  for ( const description& p : pages )
    for ( description::const_sprite_iterator it( p.sprite_begin() );
          it != p.sprite_end(); ++it )
      image_names.insert( p.images.find( it->image_id )->second );
  
  for ( const std::string& name : image_names )
    if ( !image.get_image( name )->internally_supported )
      return false;

  return true;
}
