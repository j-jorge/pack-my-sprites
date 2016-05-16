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
#include "pms/serialization/spritedesc/node_parser.hpp"

void pms::serialization::spritedesc::node_parser::error
( data_iterator first, data_iterator last, const std::string& msg ) const
{
  boost::spirit::classic::file_position pos = first.get_position();

  std::cerr << pos.file << ": " << pos.line << ": "
            << pos.column << ": " << msg << std::endl;
}