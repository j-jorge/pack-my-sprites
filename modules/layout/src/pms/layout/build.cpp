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
      std::pair< const char*, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic >
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

      struct packing
      {
        bool complete;
        std::vector< atlas_page > pages;
      };
        
      class packing_value
      {
      public:
        packing_value( bool c, std::size_t s, std::size_t p );

        bool is_better_than( const packing_value& that ) const;
        bool is_complete() const;
        
      private:
        bool complete;
        std::size_t size;
        std::size_t page_count;
      };
      
      static std::size_t packing_score
      ( const std::vector< atlas_page >& pages );
      
      static packing
      create_pages
      ( bool allow_rotate, const atlas& atlas,
        rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );
      
      static atlas_page
      create_page
      ( bool allow_rotate, std::vector< rbp::RectSize >& source,
        atlas_page& all_sprites, const atlas& atlas,
        rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );

      static atlas_page
      apply_heuristic
      ( bool allow_rotate, std::vector< rbp::RectSize >& source,
        atlas_page& all_sprites, const atlas& atlas,
        rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );

      static std::vector<rbp::Rect> try_heuristic
      ( bool allow_rotate, std::vector< rbp::RectSize > &source, int width,
        int height, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic );

      static atlas_page apply_positions
      ( atlas_page& desc, const std::vector< rbp::Rect >& packing,
        std::size_t margin );
      static void place_sprite_from_packing
      ( std::vector< atlas_page::sprite >& result,
        claw::math::coordinate_2d< int >& final_size, atlas_page& desc,
        const rbp::Rect& packing, std::size_t margin );

      static std::vector<rbp::RectSize>
      build_sprite_sizes( const atlas_page& desc, std::size_t margin );
    
      static atlas_page::sprite_iterator
      find_sprite_by_size( atlas_page& desc, int width, int height );
    }
  }
}

pms::layout::detail::packing_value::packing_value
( bool c, std::size_t s, std::size_t p )
  : complete( c ),
    size( s ),
    page_count( p )
{

}

bool pms::layout::detail::packing_value::is_better_than
( const packing_value& that ) const
{
  return std::make_tuple( !complete, page_count, size )
    < std::make_tuple( !that.complete, that.page_count, that.size );
}

bool pms::layout::detail::packing_value::is_complete() const
{
  return complete;
}

bool pms::layout::build( bool allow_rotate, atlas& atlas )
{
  assert( atlas.pages.size() == 1 );

  std::vector< atlas_page > result;
  detail::packing_value best_value
    ( false, std::numeric_limits< std::size_t >::max(), 0 );
  
  for ( const detail::named_heuristic* h( detail::g_heuristics );
        h->first != nullptr;
        ++h )
    {
      claw::logger << claw::log_verbose << "Packing with heuristic \""
                   << h->first << "\".\n";

      const detail::packing packing
        ( detail::create_pages( allow_rotate, atlas, h->second ) );

      const detail::packing_value value
        ( packing.complete, detail::packing_score( packing.pages ),
          packing.pages.size() );

      if ( value.is_better_than( best_value ) )
        {
          best_value = value;
          result = packing.pages;
        }
    }

  atlas.pages = result;

  return best_value.is_complete();
}

std::size_t pms::layout::detail::packing_score
( const std::vector< atlas_page >& pages )
{
  std::size_t result( 0 );

  for ( const atlas_page& page : pages )
    result += page.width * page.height;

  return result;
}

pms::layout::detail::packing
pms::layout::detail::create_pages
( bool allow_rotate, const atlas& atlas,
  rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  atlas_page all_sprites( atlas.pages[ 0 ] );
  std::vector< rbp::RectSize > regions
    ( build_sprite_sizes( all_sprites, atlas.margin ) );
  packing result;

  std::size_t initial_count;

  do
    {
      initial_count = regions.size();
      result.pages.push_back
        ( create_page( allow_rotate, regions, all_sprites, atlas, heuristic ) );
    }
  while( !regions.empty() && ( regions.size() < initial_count ) );

  result.complete = regions.empty();
  
  return result;
}

pms::layout::atlas_page
pms::layout::detail::create_page
( bool allow_rotate, std::vector< rbp::RectSize >& source,
  atlas_page& all_sprites, const atlas& atlas,
  rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  claw::logger << claw::log_verbose
               << "Setting sprite positions in sprite sheet '"
               << atlas.output_name << "', with "
               << all_sprites.sprite_count() << " sprites:\n"
               << all_sprites.to_string()
               << std::endl;

  const atlas_page result
    ( apply_heuristic( allow_rotate, source, all_sprites, atlas, heuristic ) );

  claw::logger << claw::log_verbose
               << "Final sprite sheet is:\n"
               << result.to_string();

  return result;
}

pms::layout::atlas_page
pms::layout::detail::apply_heuristic
( bool allow_rotate, std::vector< rbp::RectSize >& source,
  atlas_page& all_sprites, const atlas& atlas,
  rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  const std::size_t m( atlas.margin );

  const std::vector< rbp::Rect > packing
    ( try_heuristic
      ( allow_rotate, source, atlas.width - m, atlas.height - m, heuristic ) );

  return apply_positions( all_sprites, packing, m );
}

std::vector<rbp::Rect> pms::layout::detail::try_heuristic
( bool allow_rotate, std::vector< rbp::RectSize >& source, int width,
  int height, rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic )
{
  std::vector< rbp::Rect > packing;
  packing.reserve( source.size() );

  rbp::MaxRectsBinPack solver( allow_rotate, width, height );
  solver.Insert( source, packing, heuristic );

  return packing;
}

pms::layout::atlas_page
pms::layout::detail::apply_positions
( atlas_page& desc, const std::vector< rbp::Rect >& packing,
  std::size_t margin )
{
  std::vector< atlas_page::sprite > sprites;
  sprites.reserve( packing.size() );

  claw::math::coordinate_2d<int> final_size( 1, 1 );

  for( std::size_t i( 0 ); i != packing.size(); ++i )
    place_sprite_from_packing
      ( sprites, final_size, desc, packing[ i ], margin );

  atlas_page result;
  
  for( const auto& s : sprites )
    {
      result.images[ s.image_id ] = desc.images[ s.image_id ];
      result.add_sprite( s );
    }

  result.width = final_size.x ;
  result.height = final_size.y;

  return result;
}

void pms::layout::detail::place_sprite_from_packing
( std::vector< atlas_page::sprite >& result,
  claw::math::coordinate_2d< int >& final_size, atlas_page& desc,
  const rbp::Rect& packing, std::size_t margin )
{
  atlas_page::sprite_iterator it
    ( find_sprite_by_size
      ( desc, packing.width - margin, packing.height - margin ) );

  if ( it == desc.sprite_end() )
    {
      it =
        find_sprite_by_size
        ( desc, packing.height - margin, packing.width - margin );
      it->rotated = true;
    }
  else
    it->rotated = false;

  const std::size_t offset( ( it->bleed ? 1 : 0 ) + margin );

  it->result_box.position.set( packing.x + offset, packing.y + offset );
  final_size.x =
    std::max< int >( final_size.x, packing.x + packing.width + margin );
  final_size.y =
    std::max< int >( final_size.y, packing.y + packing.height + margin );
      
  result.push_back( *it );
  desc.erase_sprite( it );
}

std::vector< rbp::RectSize >
pms::layout::detail::build_sprite_sizes
( const atlas_page& desc, std::size_t margin )
{
  std::vector<rbp::RectSize> result;
  result.reserve( desc.sprite_count() );

  for ( atlas_page::const_sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( (it->bleed ? 2 : 0) + margin );
      const rbp::RectSize rect =
        {
          int( it->result_box.width + padding ),
          int( it->result_box.height + padding )
        }; 
      result.push_back( rect );
    }

  return result;
}

pms::layout::atlas_page::sprite_iterator
pms::layout::detail::find_sprite_by_size
( atlas_page& desc, int width, int height )
{
  for ( atlas_page::sprite_iterator it( desc.sprite_begin() );
        it != desc.sprite_end(); ++it )
    {
      const std::size_t padding( it->bleed ? 2 : 0 );
      
      if ( ( it->result_box.width == width - padding )
           && ( it->result_box.height == height - padding ) )
        return it;
    }

  return desc.sprite_end();
}
