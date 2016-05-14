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
