#include <sstream>

template< typename T, typename U >
void utest::detail::test_eq
( const T& a, const U& b, const char* file, std::size_t line )
{
  if ( a == b )
    return;
  
  std::ostringstream message;
  message << "Expected " << a << ", got " << b;
  fail( file, line, message.str() );
}

