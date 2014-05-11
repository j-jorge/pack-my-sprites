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
#include "sprite_sheet_builder.hpp"

#include <limits>

#include <claw/logger.hpp>

bool sdc::sprite_sheet_builder::sprite_area_comp::operator()
  ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const
{
  return a.result_box.area() > b.result_box.area();
}

bool sdc::sprite_sheet_builder::sprite_height_comp::operator()
  ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const
{
  return a.result_box.height > b.result_box.height;
}



sdc::spritedesc_collection
sdc::sprite_sheet_builder::build
( spritedesc_collection sprite_description ) const
{
  typedef spritedesc_collection::spritedesc_list_type::iterator iterator_type;

  for ( iterator_type it=sprite_description.sprite_sheet.begin();
        it!=sprite_description.sprite_sheet.end(); ++it )
    {
      claw::logger << claw::log_verbose
                   << "Setting sprite positions in sprite sheet '"
                   << it->output_name << "'"
                   << std::endl;

      set_sprite_position( *it );
    }

  return sprite_description;
}

void sdc::sprite_sheet_builder::set_sprite_position( spritedesc& desc ) const
{
  switch( desc.order )
    {
    case decreasing_area:
      desc.sort_sprites( sprite_area_comp() );
      break;
    case decreasing_height:
      desc.sort_sprites( sprite_height_comp() );
      break;
    case declaration_order:
      // Nothing to do: the declaration order is the right order.
      break;
    }
  
  std::list<rectangle_type> empty_places;
  empty_places.push_back
    ( rectangle_type(0, 0, desc.width, desc.height) );

  rectangle_type final_size( 0, 0, 1, 1 );
  spritedesc::sprite_iterator it=desc.sprite_begin();

  while ( it != desc.sprite_end() )
    {
      if ( find_place_for( empty_places, *it, desc.margin ) )
        {
          final_size = final_size.join( it->result_box );
          ++it;
        }
      else
        {
          const spritedesc::sprite_iterator prev( it );
          ++it;
          desc.erase_sprite( prev );
        }
    }

  desc.width = final_size.width + desc.margin;
  desc.height = final_size.height + desc.margin;
}

bool sdc::sprite_sheet_builder::find_place_for
( std::list<rectangle_type>& empty_places, spritedesc::sprite& s,
  std::size_t m ) const
{
  std::size_t area = std::numeric_limits<std::size_t>::max();
  std::list<rectangle_type>::iterator selected = empty_places.end();
  std::list<rectangle_type> failed;

  const std::size_t w( s.result_box.width + m );
  const std::size_t h( s.result_box.height + m );

  for ( std::list<rectangle_type>::iterator it = empty_places.begin();
        it != empty_places.end(); ++it )
    if ( (it->width >= w) && (it->height >= h) )
      {
        if ( it->area() < area )
          {
            area = it->area();
            selected = it;
          }
      }
    else
      failed.push_back(*it);

  if ( selected == empty_places.end() )
    {
      std::cerr << "Can't find a place for sprite " << s.name 
                << " (with a size equal to " << s.result_box.width << 'x'
                << s.result_box.height << " and an initial size equal to "
                << s.source_box.width << 'x' << s.source_box.height << ")."
                << std::endl;

      for ( std::list<rectangle_type>::iterator it = failed.begin();
            it != failed.end(); ++it )
        std::cerr << "\t(" << (int)(w - it->width) << ", "
                  << (int)(h - it->height) << ')' << std::endl;

      return false;
    }
  else
    {
      const rectangle_type r = *selected;
      empty_places.erase(selected);

      s.result_box.position = r.position;

      const rectangle_type r_right
        ( r.left() + w, r.top(), r.width - w, h );
      const rectangle_type r_bottom
        ( r.left(), r.top() + h, r.width, r.height - h );

      CLAW_POSTCOND( r.includes( r_right ) );
      CLAW_POSTCOND( r.includes( r_bottom ) );

      if ( r_right.area() != 0 )
        empty_places.push_back( r_right );

      if ( r_bottom.area() != 0 )
        empty_places.push_back( r_bottom );

      return true;
    }
}
