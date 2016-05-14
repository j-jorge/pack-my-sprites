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
#include "pms/layout/build.hpp"

#include "rbp/MaxRectsBinPack.h"

#include <limits>

#include <claw/logger.hpp>

namespace pms
{
  namespace layout
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
          named_heuristic( nullptr, rbp::MaxRectsBinPack::RectContactPointRule )
        };

      static bool set_sprite_positions( bool allow_rotate, description& desc );
      static bool try_all_heuristics( bool allow_rotate, description& desc );

      static bool try_heuristic
      ( bool allow_rotate, const std::vector<rbp::RectSize>& source,
        description& desc,
        rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );
      static std::vector<rbp::Rect> try_heuristic
      ( bool allow_rotate, std::vector<rbp::RectSize> source, int width,
        int height, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );

      static void apply_positions
      ( description& desc, const std::vector<rbp::Rect>& packing );
      static void place_sprite_from_packing
      ( std::vector<description::sprite>& result,
        claw::math::coordinate_2d<int>& final_size, description& desc,
        const rbp::Rect& packing );

      static std::vector<rbp::RectSize>
      build_sprite_sizes( const description& desc );
    
      static description::sprite_iterator
      find_sprite_by_size( description& desc, int width, int height );
    }
  }
}

bool pms::layout::build( bool allow_rotate, description& desc )
{
  claw::logger << claw::log_verbose
               << "Setting sprite positions in sprite sheet '"
               << desc.output_name << "'"
               << std::endl;

  const bool result( detail::set_sprite_positions( allow_rotate, desc ) );

  claw::logger << claw::log_verbose
               << "Final sprite sheet is:\n"
               << desc.to_string();

  return result;
}

bool pms::layout::detail::set_sprite_positions
( bool allow_rotate, description& desc )
{
  if ( desc.margin >= desc.width )
    {
      std::cerr << "Fatal: the margin is larger than the expected width.\n";
      return false;
    }

  if ( desc.margin >= desc.height )
    {
      std::cerr << "Fatal: the margin is larger than the expected height.\n";
      return false;
    }

  return try_all_heuristics( allow_rotate, desc );
}

bool pms::layout::detail::try_all_heuristics
( bool allow_rotate, description& desc )
{
  const std::vector< rbp::RectSize > source( build_sprite_sizes( desc ) );

  for ( const named_heuristic* h = g_heuristics; h->first != nullptr; ++h )
    {
      claw::logger << claw::log_verbose << "Packing with heuristic \""
                   << h->first << "\".\n";
      if ( try_heuristic( allow_rotate, source, desc, h->second ) )
        return true;
    }

  std::cerr << "Could not place all sprites.\n";
  return false;
}

bool pms::layout::detail::try_heuristic
( bool allow_rotate, const std::vector< rbp::RectSize >& source,
  description& desc, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  const std::size_t m( desc.margin );

  const std::vector< rbp::Rect > packing
    ( try_heuristic
      ( allow_rotate, source, desc.width - m, desc.height - m, heuristic ) );

  if ( packing.size() != source.size() )
    return false;
  
  apply_positions( desc, packing );
  
  return true;
}

std::vector<rbp::Rect> pms::layout::detail::try_heuristic
( bool allow_rotate, std::vector< rbp::RectSize > source, int width, int height,
  rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  std::vector< rbp::Rect > packing;
  packing.reserve( source.size() );

  rbp::MaxRectsBinPack solver( allow_rotate, width, height );
  solver.Insert( source, packing, heuristic );

  return packing;
}

void pms::layout::detail::apply_positions
( description& desc, const std::vector< rbp::Rect >& packing )
{
  std::vector< description::sprite > result;
  result.reserve( packing.size() );

  claw::math::coordinate_2d<int> final_size( 1, 1 );

  for( std::size_t i( 0 ); i != packing.size(); ++i )
      place_sprite_from_packing( result, final_size, desc, packing[ i ] );

  for( std::size_t i( 0 ); i != result.size(); ++i )
    desc.add_sprite( result[ i ] );

  desc.width = final_size.x ;
  desc.height = final_size.y;
}

void pms::layout::detail::place_sprite_from_packing
( std::vector< description::sprite >& result,
  claw::math::coordinate_2d< int >& final_size, description& desc,
  const rbp::Rect& packing )
{
  description::sprite_iterator it
    ( find_sprite_by_size( desc, packing.width, packing.height ) );

  if ( it == desc.sprite_end() )
    {
      it = find_sprite_by_size( desc, packing.height, packing.width );
      it->rotated = true;
    }
  else
    it->rotated = false;

  const int m( desc.margin );
  const std::size_t offset( ( it->bleed ? 1 : 0 ) + m );

  it->result_box.position.set( packing.x + offset, packing.y + offset );
  final_size.x = std::max( final_size.x, packing.x + packing.width + m );
  final_size.y = std::max( final_size.y, packing.y + packing.height + m );
      
  result.push_back( *it );
  desc.erase_sprite( it );
}

std::vector< rbp::RectSize >
pms::layout::detail::build_sprite_sizes( const description& desc )
{
  const std::size_t m( desc.margin );

  std::vector<rbp::RectSize> result;
  result.reserve( desc.sprite_count() );

  for ( description::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( (it->bleed ? 2 : 0) + m );
      const rbp::RectSize rect =
        {
          int( it->result_box.width + padding ),
          int( it->result_box.height + padding )
        }; 
      result.push_back( rect );
    }

  return result;
}

pms::layout::description::sprite_iterator
pms::layout::detail::find_sprite_by_size
( description& desc, int width, int height )
{
  width -= desc.margin;
  height -= desc.margin;
  
  for ( description::sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( it->bleed ? 2 : 0 );
      
      if ( ( it->result_box.width == width - padding )
           && ( it->result_box.height == height - padding ) )
        return it;
    }

  return desc.sprite_end();
}
