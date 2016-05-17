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
#pragma once

#include <iostream>

namespace utest
{
  namespace detail
  {
    void fail( const char* file, std::size_t line, const std::string& message );
    void fail( const char* file, std::size_t line );

    template< typename T, typename U >
    void test_eq( const T& a, const U& b, const char* file, std::size_t line );
  }
}

#define TEST( b ) do                                                    \
    {                                                                   \
      if ( !(b) )                                                       \
        ::utest::detail::fail( __FILE__, __LINE__ );                    \
    } while ( false )

#define TEST_EQ( a, b )                                                 \
    ::utest::detail::test_eq( (a), (b), __FILE__, __LINE__ );           \

#define TEST_THROWS( c )                                \
  do {                                                  \
    try                                                 \
      {                                                 \
        c;                                              \
        ::utest::detail::fail( __FILE__, __LINE__ );    \
      }                                                 \
    catch ( const std::exception& e )                   \
      {                                                 \
      }                                                 \
  } while ( false )
    
#include "utest/utest.tpp"
