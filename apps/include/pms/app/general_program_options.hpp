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
#pragma once

#include <boost/program_options.hpp>

namespace pms
{
  namespace app
  {
    boost::program_options::options_description get_general_program_options();

    bool parse_general_program_options
    ( const boost::program_options::options_description& options,
      const boost::program_options::variables_map& values,
      const std::string& program_name, const std::string& arguments_string );
  }
}

