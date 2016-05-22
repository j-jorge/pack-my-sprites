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

#include "utest/utest.hpp"

#include <algorithm>
#include <tuple>

void print
( const pms::layout::description& description,
  const std::vector< pms::layout::description::sprite >& sprites )
{
  for ( std::size_t cy( 0 ); cy != description.height; ++cy )
    {
      for ( std::size_t cx( 0 ); cx != description.width; )
        {
          bool p( false );
          
          for ( const pms::layout::description::sprite& sprite : sprites )
            if ( ( sprite.result_box.position.x == cx )
                 && ( sprite.result_box.position.y <= cy )
                 && ( cy - sprite.result_box.position.y
                      < sprite.result_box.height ) )
              {
                p = true;
                cx += sprite.result_box.width;
                
                for ( int i( 0 ); i != sprite.result_box.width; ++i )
                  std::cout << sprite.name.back();
              }

          if ( !p )
            {
              std::cout << '.';
              ++cx;
            }
        }

      std::cout << '\n';
    }
}

pms::layout::description::sprite create_sprite
( const std::string& id, const std::string name, int width, int height,
  bool bleed )
{
  pms::layout::description::sprite result;
  result.image_id = id;
  result.name = name;
  result.source_box.set( 0, 0, 10, 20 );
  result.result_box.set( 0, 0, width, height );
  result.bleed = bleed;

  return result;
}

void empty()
{
  pms::layout::atlas atlas;
  atlas.width = 200;
  atlas.height = 200;
  atlas.margin = 0;
  atlas.pages.emplace_back();
  
  TEST( pms::layout::build( false, atlas ) );
  
  TEST_EQ( 1, atlas.pages.size() );
  TEST_EQ( 0, atlas.pages[ 0 ].sprite_count() );
}

void fit_one()
{
  pms::layout::atlas atlas;
  atlas.width = 200;
  atlas.height = 300;
  atlas.margin = 0;
  
  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";
  description.add_sprite( create_sprite( "a", "b", 200, 300, false ) );

  atlas.pages.push_back( description );
  
  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  const auto it( description.get_sprite_by_name( "b" ) );
  
  TEST( it->result_box.position.x == 0 );
  TEST( it->result_box.position.y == 0 );
  TEST( it->result_box.width == 200 );
  TEST( it->result_box.height == 300 );
}

void fit_rotate()
{
  pms::layout::atlas atlas;
  atlas.width = 300;
  atlas.height = 200;
  atlas.margin = 0;
  
  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";
  description.add_sprite( create_sprite( "a", "b", 200, 300, false ) );
  
  atlas.pages.push_back( description );
  
  TEST( pms::layout::build( true, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  const auto it( atlas.pages[ 0 ].get_sprite_by_name( "b" ) );
  
  TEST( it->result_box.position.x == 0 );
  TEST( it->result_box.position.y == 0 );
  TEST( it->result_box.width == 200 );
  TEST( it->result_box.height == 300 );
  TEST( it->rotated );
}

void forbid_rotate()
{
  pms::layout::atlas atlas;
  atlas.width = 300;
  atlas.height = 200;
  atlas.margin = 0;
  
  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";
  description.add_sprite( create_sprite( "a", "b", 200, 300, false ) );
  
  atlas.pages.push_back( description );

  TEST( !pms::layout::build( false, atlas ) );
}

void final_size()
{
  pms::layout::atlas atlas;
  atlas.width = 2000;
  atlas.height = 3000;
  atlas.margin = 0;
  
  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";
  description.add_sprite( create_sprite( "a", "b", 200, 300, false ) );
  
  atlas.pages.push_back( description );

  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  TEST_EQ( 200, atlas.pages[ 0 ].width );
  TEST_EQ( 300, atlas.pages[ 0 ].height );
}

void rotate()
{
  pms::layout::atlas atlas;
  atlas.width = 40;
  atlas.height = 40;
  atlas.margin = 0;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 20, 10, false ) );
  description.add_sprite( create_sprite( "a", "s2", 11, 21, false ) );
  description.add_sprite( create_sprite( "a", "s3", 22, 12, false ) );
  description.add_sprite( create_sprite( "a", "s4", 13, 23, false ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( true, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  std::vector< pms::layout::description::sprite > sprites
    ( description.sprite_begin(), description.sprite_end() );
  std::sort
    ( sprites.begin(), sprites.end(),
      []( const pms::layout::description::sprite& a,
          const pms::layout::description::sprite& b ) -> bool
      {
        return
          std::make_tuple( a.result_box.position.x, a.result_box.position.y )
          < std::make_tuple( b.result_box.position.x, b.result_box.position.y );
      } );

  pms::layout::description::sprite& s1( sprites[ 3 ] );
  const pms::layout::description::sprite& s2( sprites[ 2 ] );
  const pms::layout::description::sprite& s3( sprites[ 1 ] );
  const pms::layout::description::sprite& s4( sprites[ 0 ] );

  TEST( s1.rotated );
  TEST( !s2.rotated );
  TEST( !s3.rotated );
  TEST( !s4.rotated );

  std::swap( s1.result_box.width, s1.result_box.height );

  TEST_EQ( 34, description.width );
  TEST_EQ( 35, description.height );

  TEST_EQ( 10, s1.result_box.width );
  TEST_EQ( 11, s2.result_box.width );
  TEST_EQ( 22, s3.result_box.width );
  TEST_EQ( 13, s4.result_box.width );

  TEST_EQ( 0, s4.result_box.position.x );
  TEST_EQ( 0, s4.result_box.position.y );
  
  TEST_EQ( 13, s2.result_box.position.x );
  TEST_EQ( 0, s2.result_box.position.y );
  
  TEST_EQ( 24, s1.result_box.position.x );
  TEST_EQ( 0, s1.result_box.position.y );
  
  TEST_EQ( 0, s3.result_box.position.x );
  TEST_EQ( 23, s3.result_box.position.y );
}

void margin()
{
  pms::layout::atlas atlas;
  atlas.width = 30;
  atlas.height = 60;
  atlas.margin = 2;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 10, 20, false ) );
  description.add_sprite( create_sprite( "a", "s2", 10, 20, false ) );
  description.add_sprite( create_sprite( "a", "s3", 10, 20, false ) );
  description.add_sprite( create_sprite( "a", "s4", 10, 20, false ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  TEST_EQ( 26, description.width );
  TEST_EQ( 46, description.height );

  std::vector< pms::layout::description::sprite > sprites
    ( description.sprite_begin(), description.sprite_end() );
  std::sort
    ( sprites.begin(), sprites.end(),
      []( const pms::layout::description::sprite& a,
          const pms::layout::description::sprite& b ) -> bool
      {
        return
          std::make_tuple( a.result_box.position.x, a.result_box.position.y )
          < std::make_tuple( b.result_box.position.x, b.result_box.position.y );
      } );

  TEST( !sprites[ 0 ].result_box.intersects( sprites[ 1 ].result_box ) );
  TEST( !sprites[ 0 ].result_box.intersects( sprites[ 2 ].result_box ) );
  TEST( !sprites[ 0 ].result_box.intersects( sprites[ 3 ].result_box ) );

  TEST( !sprites[ 1 ].result_box.intersects( sprites[ 2 ].result_box ) );
  TEST( !sprites[ 1 ].result_box.intersects( sprites[ 3 ].result_box ) );

  TEST( !sprites[ 2 ].result_box.intersects( sprites[ 3 ].result_box ) );

  TEST( sprites[ 0 ].result_box.position.x == 2 );
  TEST( sprites[ 0 ].result_box.position.y == 2 );
  
  TEST( sprites[ 1 ].result_box.position.x == 2 );
  TEST( sprites[ 1 ].result_box.position.y == 24 );
  
  TEST( sprites[ 2 ].result_box.position.x == 14 );
  TEST( sprites[ 2 ].result_box.position.y == 2 );
  
  TEST( sprites[ 3 ].result_box.position.x == 14 );
  TEST( sprites[ 3 ].result_box.position.y == 24 );
}

void bleeding()
{
  pms::layout::atlas atlas;
  atlas.width = 60;
  atlas.height = 30;
  atlas.margin = 0;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 20, 10, true ) );
  description.add_sprite( create_sprite( "a", "s2", 21, 11, false ) );
  description.add_sprite( create_sprite( "a", "s3", 22, 12, false ) );
  description.add_sprite( create_sprite( "a", "s4", 23, 13, true ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  /*
    4444444444444444444444444.....................
    4.......................4.                   .
    4.                     .4.                   .
    4.                     .4.                   .
    4.                     .4.                   .
    4.                     .4.       s2          .
    4.                     .4.                   .
    4.         s4          .4.                   .
    4.                     .4.                   .
    4.                     .4.                   .
    4.                     .4.....................
    4.                     .4
    4.                     .4
    4.......................4
    4444444444444444444444444
    1111111111111111111111......................
    1....................1.                    .
    1.                  .1.                    .
    1.                  .1.                    .
    1.                  .1.                    .
    1.       s1         .1.        s3          .
    1.                  .1.                    .
    1.                  .1.                    .
    1.                  .1.                    .
    1.                  .1.                    .
    1....................1.                    .
    1111111111111111111111......................
  */

  std::vector< pms::layout::description::sprite > sprites
    ( description.sprite_begin(), description.sprite_end() );
  std::sort
    ( sprites.begin(), sprites.end(),
      []( const pms::layout::description::sprite& a,
          const pms::layout::description::sprite& b ) -> bool
      {
        return
          std::make_tuple( a.result_box.position.x, a.result_box.position.y )
          < std::make_tuple( b.result_box.position.x, b.result_box.position.y );
      } );

  const pms::layout::description::sprite& s1( sprites[ 1 ] );
  const pms::layout::description::sprite& s2( sprites[ 3 ] );
  const pms::layout::description::sprite& s3( sprites[ 2 ] );
  const pms::layout::description::sprite& s4( sprites[ 0 ] );
  
  TEST( !s1.result_box.intersects( s2.result_box ) );
  TEST( !s1.result_box.intersects( s3.result_box ) );
  TEST( !s1.result_box.intersects( s4.result_box ) );

  TEST( !s2.result_box.intersects( s3.result_box ) );
  TEST( !s2.result_box.intersects( s4.result_box ) );

  TEST( !s3.result_box.intersects( s4.result_box ) );

  TEST_EQ( 20, s1.result_box.width );
  TEST_EQ( 21, s2.result_box.width );
  TEST_EQ( 22, s3.result_box.width );
  TEST_EQ( 23, s4.result_box.width );

  TEST_EQ( 46, description.width );
  TEST_EQ( 27, description.height );

  TEST_EQ( 1, s4.result_box.position.x );
  TEST_EQ( 1, s4.result_box.position.y );
  
  TEST_EQ( 25, s2.result_box.position.x );
  TEST_EQ( 0, s2.result_box.position.y );
  
  TEST_EQ( 1, s1.result_box.position.x );
  TEST_EQ( 16, s1.result_box.position.y );
  
  TEST_EQ( 22, s3.result_box.position.x );
  TEST_EQ( 15, s3.result_box.position.y );
}

void margin_and_bleeding()
{
  pms::layout::atlas atlas;
  atlas.width = 60;
  atlas.height = 30;
  atlas.margin = 1;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 20, 10, true ) );
  description.add_sprite( create_sprite( "a", "s2", 21, 11, false ) );
  description.add_sprite( create_sprite( "a", "s3", 22, 12, false ) );
  description.add_sprite( create_sprite( "a", "s4", 23, 13, true ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  /*
   _________________________________________________
   _4444444444444444444444444_....................._
   _4.......................4_.                   ._
   _4.                     .4_.                   ._
   _4.                     .4_.                   ._
   _4.                     .4_.                   ._
   _4.                     .4_.       s2          ._
   _4.                     .4_.                   ._
   _4.         s4          .4_.                   ._
   _4.                     .4_.                   ._
   _4.                     .4_.                   ._
   _4.                     .4_....................._
   _4.                     .4_                     _
   _4.                     .4_                     _
   _4.......................4_                     _
   _4444444444444444444444444_                     _
   _________________________________________________
   _1111111111111111111111_......................  _
   _1....................1_.                    .  _
   _1.                  .1_.                    .  _
   _1.                  .1_.                    .  _
   _1.                  .1_.                    .  _
   _1.       s1         .1_.        s3          .  _
   _1.                  .1_.                    .  _
   _1.                  .1_.                    .  _
   _1.                  .1_.                    .  _
   _1.                  .1_.                    .  _
   _1....................1_.                    .  _
   _1111111111111111111111_......................  _
   _________________________________________________
  */

  std::vector< pms::layout::description::sprite > sprites
    ( description.sprite_begin(), description.sprite_end() );
  std::sort
    ( sprites.begin(), sprites.end(),
      []( const pms::layout::description::sprite& a,
          const pms::layout::description::sprite& b ) -> bool
      {
        return
          std::make_tuple( a.result_box.position.x, a.result_box.position.y )
          < std::make_tuple( b.result_box.position.x, b.result_box.position.y );
      } );

  const pms::layout::description::sprite& s1( sprites[ 1 ] );
  const pms::layout::description::sprite& s2( sprites[ 3 ] );
  const pms::layout::description::sprite& s3( sprites[ 2 ] );
  const pms::layout::description::sprite& s4( sprites[ 0 ] );
  
  TEST( !s1.result_box.intersects( s2.result_box ) );
  TEST( !s1.result_box.intersects( s3.result_box ) );
  TEST( !s1.result_box.intersects( s4.result_box ) );

  TEST( !s2.result_box.intersects( s3.result_box ) );
  TEST( !s2.result_box.intersects( s4.result_box ) );

  TEST( !s3.result_box.intersects( s4.result_box ) );

  TEST_EQ( 20, s1.result_box.width );
  TEST_EQ( 21, s2.result_box.width );
  TEST_EQ( 22, s3.result_box.width );
  TEST_EQ( 23, s4.result_box.width );

  TEST_EQ( 49, description.width );
  TEST_EQ( 30, description.height );

  TEST_EQ( 2, s4.result_box.position.x );
  TEST_EQ( 2, s4.result_box.position.y );
  
  TEST_EQ( 27, s2.result_box.position.x );
  TEST_EQ( 1, s2.result_box.position.y );
  
  TEST_EQ( 2, s1.result_box.position.x );
  TEST_EQ( 18, s1.result_box.position.y );
  
  TEST_EQ( 24, s3.result_box.position.x );
  TEST_EQ( 17, s3.result_box.position.y );
}

void rotate_and_margin_and_bleeding()
{
  pms::layout::atlas atlas;
  atlas.width = 40;
  atlas.height = 40;
  atlas.margin = 1;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 20, 10, true ) );
  description.add_sprite( create_sprite( "a", "s2", 11, 21, false ) );
  description.add_sprite( create_sprite( "a", "s3", 22, 12, false ) );
  description.add_sprite( create_sprite( "a", "s4", 13, 23, true ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( true, atlas ) );
  TEST_EQ( 1, atlas.pages.size() );

  description = atlas.pages[ 0 ];

  std::vector< pms::layout::description::sprite > sprites
    ( description.sprite_begin(), description.sprite_end() );
  std::sort
    ( sprites.begin(), sprites.end(),
      []( const pms::layout::description::sprite& a,
          const pms::layout::description::sprite& b ) -> bool
      {
        return
          std::make_tuple( a.result_box.position.x, a.result_box.position.y )
          < std::make_tuple( b.result_box.position.x, b.result_box.position.y );
      } );

  const pms::layout::description::sprite& s1( sprites[ 1 ] );
  pms::layout::description::sprite& s2( sprites[ 3 ] );
  const pms::layout::description::sprite& s3( sprites[ 2 ] );
  const pms::layout::description::sprite& s4( sprites[ 0 ] );

  TEST( s2.rotated );

  std::swap( s2.result_box.width, s2.result_box.height );
  
  TEST_EQ( 40, description.width );
  TEST_EQ( 40, description.height );

  TEST_EQ( 20, s1.result_box.width );
  TEST_EQ( 21, s2.result_box.width );
  TEST_EQ( 22, s3.result_box.width );
  TEST_EQ( 13, s4.result_box.width );

  TEST_EQ( 2, s4.result_box.position.x );
  TEST_EQ( 2, s4.result_box.position.y );
  
  TEST_EQ( 17, s2.result_box.position.x );
  TEST_EQ( 14, s2.result_box.position.y );
  
  TEST_EQ( 2, s1.result_box.position.x );
  TEST_EQ( 28, s1.result_box.position.y );
  
  TEST_EQ( 17, s3.result_box.position.x );
  TEST_EQ( 1, s3.result_box.position.y );
}

void multiple_output()
{
  pms::layout::atlas atlas;
  atlas.width = 20;
  atlas.height = 20;
  atlas.margin = 0;

  pms::layout::description description;
  description.images[ "a" ] = "path/to/a";

  description.add_sprite( create_sprite( "a", "s1", 20, 15, false ) );
  description.add_sprite( create_sprite( "a", "s2", 15, 20, false ) );

  atlas.pages.push_back( description );

  TEST( pms::layout::build( false, atlas ) );
  TEST_EQ( 2, atlas.pages.size() );

  pms::layout::description d1( atlas.pages[ 0 ] );
  pms::layout::description d2( atlas.pages[ 1 ] );

  TEST_EQ( 1, d1.images.size() );
  TEST_EQ( d1.images.size(), d2.images.size() );
  
  TEST_EQ( 20, d1.width );
  TEST_EQ( 15, d1.height );

  TEST_EQ( 15, d2.width );
  TEST_EQ( 20, d2.height );

  const pms::layout::description::sprite& s1( *d1.sprite_begin() );

  TEST( !s1.rotated );
  TEST_EQ( 20, s1.result_box.width );
  TEST_EQ( 15, s1.result_box.height );
  TEST_EQ( 0, s1.result_box.position.x );
  TEST_EQ( 0, s1.result_box.position.y );
  
  const pms::layout::description::sprite& s2( *d2.sprite_begin() );

  TEST( !s2.rotated );
  TEST_EQ( 15, s2.result_box.width );
  TEST_EQ( 20, s2.result_box.height );
  TEST_EQ( 0, s2.result_box.position.x );
  TEST_EQ( 0, s2.result_box.position.y );
}

int main()
{
  empty();
  fit_one();
  fit_rotate();
  forbid_rotate();
  final_size();
  rotate();
  
  margin();
  bleeding();
  margin_and_bleeding();
  
  rotate_and_margin_and_bleeding();

  multiple_output();
    
  return 0;
}
