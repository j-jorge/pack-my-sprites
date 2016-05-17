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
