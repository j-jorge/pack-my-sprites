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

namespace pms
{
  namespace detail
  {
    typedef
    std::pair<const char*, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic>
    named_heuristic;

    static const named_heuristic g_heuristics[] =
      {
        named_heuristic
        ( "Best short size fit", rbp::MaxRectsBinPack::RectBestShortSideFit ),
        named_heuristic
        ( "Best long size fit", rbp::MaxRectsBinPack::RectBestLongSideFit ),
        named_heuristic
        ( "Best area fit", rbp::MaxRectsBinPack::RectBestAreaFit ),
        named_heuristic
        ( "Bottom left rule", rbp::MaxRectsBinPack::RectBottomLeftRule ),
        named_heuristic
        ( "Contact point rule", rbp::MaxRectsBinPack::RectContactPointRule ),
        named_heuristic( NULL, rbp::MaxRectsBinPack::RectContactPointRule )
      };

    static void set_sprite_positions( bool allow_rotate, spritedesc& desc );
    static void try_all_heuristics( bool allow_rotate, spritedesc& desc );

    static bool try_heuristic
    ( bool allow_rotate, const std::vector<rbp::RectSize>& source,
      spritedesc& desc,
      rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );
    static std::vector<rbp::Rect> try_heuristic
    ( bool allow_rotate, std::vector<rbp::RectSize> source, int width,
      int height, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );

    static void apply_positions
    ( spritedesc& desc, const std::vector<rbp::Rect>& packing );
    static void place_sprite_from_packing
    ( std::vector<spritedesc::sprite>& result,
      claw::math::coordinate_2d<int>& final_size, spritedesc& desc,
      const rbp::Rect& packing );

    static std::vector<rbp::RectSize>
    build_sprite_sizes( const spritedesc& desc );
    
    static spritedesc::sprite_iterator
    find_sprite_by_size( spritedesc& desc, int width, int height );
  }
}


pms::sprite_sheet
pms::sprite_sheet_builder::build
( bool allow_rotate, sprite_sheet sprite_description ) const
{
  claw::logger << claw::log_verbose
               << "Setting sprite positions in sprite sheet '"
               << sprite_description.description.output_name << "'"
               << std::endl;

  detail::set_sprite_positions( allow_rotate, sprite_description.description );

  claw::logger << claw::log_verbose
               << "Final sprite sheet is:\n"
               << sprite_description.description.to_string();
  
  return sprite_description;
}

static void pms::detail::set_sprite_positions
( bool allow_rotate, spritedesc& desc )
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

  try_all_heuristics( allow_rotate, desc );
}

static void pms::detail::try_all_heuristics
( bool allow_rotate, spritedesc& desc )
{
  const std::vector<rbp::RectSize> source( build_sprite_sizes( desc ) );

  for ( const named_heuristic* h = g_heuristics; h->first != NULL; ++h )
    {
      claw::logger << claw::log_verbose << "Packing with heuristic \""
                   << h->first << "\".\n";
      if ( try_heuristic( allow_rotate, source, desc, h->second ) )
        return;
    }

  std::cerr << "Could not place all sprites.\n";
}

static bool pms::detail::try_heuristic
( bool allow_rotate, const std::vector<rbp::RectSize>& source,
  spritedesc& desc, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  const std::size_t m( desc.margin );

  const std::vector<rbp::Rect> packing
    ( try_heuristic
      ( allow_rotate, source, desc.width - m, desc.height - m, heuristic ) );

  if ( packing.size() != source.size() )
    return false;
  
  apply_positions( desc, packing );
  
  return true;
}

static std::vector<rbp::Rect> pms::detail::try_heuristic
( bool allow_rotate, std::vector<rbp::RectSize> source, int width, int height,
  rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  std::vector<rbp::Rect> packing;
  packing.reserve( source.size() );

  rbp::MaxRectsBinPack solver( allow_rotate, width, height );
  solver.Insert( source, packing, heuristic );

  return packing;
}

static void pms::detail::apply_positions
( spritedesc& desc, const std::vector<rbp::Rect>& packing )
{
  std::vector<spritedesc::sprite> result;
  result.reserve( packing.size() );

  claw::math::coordinate_2d<int> final_size( 1, 1 );

  for( std::size_t i( 0 ); i != packing.size(); ++i )
      place_sprite_from_packing( result, final_size, desc, packing[ i ] );

  for( std::size_t i( 0 ); i != result.size(); ++i )
    desc.add_sprite( result[ i ] );

  desc.width = final_size.x ;
  desc.height = final_size.y;
}

static void pms::detail::place_sprite_from_packing
( std::vector<spritedesc::sprite>& result,
  claw::math::coordinate_2d<int>& final_size, spritedesc& desc,
  const rbp::Rect& packing )
{
  spritedesc::sprite_iterator it
    ( find_sprite_by_size( desc, packing.width, packing.height ) );

  if ( it == desc.sprite_end() )
    {
      it = find_sprite_by_size( desc, packing.height, packing.width );
      it->rotated = true;
    }
  else
    it->rotated = false;

  const std::size_t offset( it->bleed ? 1 : 0 );

  it->result_box.position.set( packing.x + offset, packing.y + offset );
  final_size.x = std::max( final_size.x, packing.x + packing.width );
  final_size.y = std::max( final_size.y, packing.y + packing.height );
      
  result.push_back( *it );
  desc.erase_sprite( it );
}

static std::vector<rbp::RectSize>
pms::detail::build_sprite_sizes( const spritedesc& desc )
{
  const std::size_t m( desc.margin );

  std::vector<rbp::RectSize> result;
  result.reserve( desc.sprite_count() );

  for ( spritedesc::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( (it->bleed ? 2 : 0) + m );
      const rbp::RectSize rect =
        { it->result_box.width + padding, it->result_box.height + padding }; 
      result.push_back( rect );
    }

  return result;
}

static pms::spritedesc::sprite_iterator
pms::detail::find_sprite_by_size( spritedesc& desc, int width, int height )
{
  width -= desc.margin;
  height -= desc.margin;
  
  for ( spritedesc::sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( it->bleed ? 2 : 0 );
      
      if ( ( it->result_box.width == width - padding )
           && ( it->result_box.height == height - padding ) )
        return it;
    }

  return desc.sprite_end();
}
