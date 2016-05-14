#include "utest/utest.hpp"

void utest::detail::fail
( const char* file, std::size_t line, const std::string& message )
{
  std::cerr << "\033[1;31m" << "Test failed in '" << file << "', line " << line
            << ": " << message << "\033[0m\n";
  std::abort();
}

void utest::detail::fail( const char* file, std::size_t line )
{
  std::cerr << "\033[1;31m" << "Test failed in '" << file << "', line " << line
            << "\033[0m\n";
  std::abort();
}
