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

#include "MaxRectsBinPack.h"

#include <limits>

#include <claw/logger.hpp>

pms::sprite_sheet
pms::sprite_sheet_builder::build( sprite_sheet sprite_description ) const
{
  claw::logger << claw::log_verbose
               << "Setting sprite positions in sprite sheet '"
               << sprite_description.description.output_name << "'"
               << std::endl;

  set_sprite_position( sprite_description.description );

  claw::logger << claw::log_verbose
               << "Final sprite sheet is:\n"
               << sprite_description.description.to_string();
  
  return sprite_description;
}

void pms::sprite_sheet_builder::set_sprite_position( spritedesc& desc ) const
{
  if ( desc.margin >= desc.width )
    {
      std::cerr << "Fatal: the margin is larger than the expected width.\n";
      return;
    }

  if ( desc.margin >= desc.height )
    {
      std::cerr << "Fatal: the margin is larger than the expected height.\n";
      return;
    }

  const std::size_t m( desc.margin );

  std::vector<rbp::RectSize> source;
  source.reserve( desc.sprite_count() );

  for ( spritedesc::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const rbp::RectSize rect =
        { it->result_box.width + m, it->result_box.height + m }; 
      source.push_back( rect );
    }

  std::vector<rbp::Rect> packed;
  packed.reserve( source.size() );

  rbp::MaxRectsBinPack solver( false, desc.width - m, desc.height - m );
  solver.Insert( source, packed, rbp::MaxRectsBinPack::RectContactPointRule );

  if ( packed.size() != desc.sprite_count() )
    {
      claw::logger << claw::log_verbose
                   << "Could not place all sprites, only " << packed.size()
                   << " out of " << source.size() << '\n';
      return;
    }

  std::vector<spritedesc::sprite> result;
  result.reserve( packed.size() );

  claw::math::rectangle<int> final_size( 0, 0, 1, 1 );

  for( std::size_t i( 0 ); i != packed.size(); ++i )
    {
      spritedesc::sprite_iterator it
        ( find_sprite_by_size
          ( desc, packed[ i ].width - m, packed[ i ].height - m ) );
      it->result_box.position.set( packed[ i ].x, packed[ i ].y );
      final_size = final_size.join( it->result_box );
      result.push_back( *it );
      desc.erase_sprite( it );
    }

  for( std::size_t i( 0 ); i != result.size(); ++i )
    desc.add_sprite( result[ i ] );

  desc.width = final_size.width + m;
  desc.height = final_size.height + m;
}

pms::spritedesc::sprite_iterator
pms::sprite_sheet_builder::find_sprite_by_size
( spritedesc& desc, int width, int height ) const
{
  spritedesc::sprite_iterator rotated( desc.sprite_end() );

  for ( spritedesc::sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    if ( ( it->result_box.width == width )
         && ( it->result_box.height == height ) )
      return it;
    else if ( ( it->result_box.width == height )
         && ( it->result_box.height == width ) )
      rotated = it;

  return rotated;
}
