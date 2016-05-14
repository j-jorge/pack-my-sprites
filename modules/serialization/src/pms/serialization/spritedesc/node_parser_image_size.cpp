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
#include "pms/serialization/spritedesc/node_parser_image_size.hpp"

#include "pms/layout/description.hpp"

void pms::serialization::spritedesc::node_parser_image_size::parse_node
( layout::description::sprite& s, const tree_node& node ) const
{
  const std::string input( node.value.begin(), node.value.end() );
  std::istringstream iss( input );
  double ratio;

  if ( iss >> ratio )
    {
      s.result_box.width = s.source_box.width * ratio;
      s.result_box.height = s.source_box.height * ratio;
    }
  else
    std::cerr << "invalid ratio '" << input
              << "' for sprite '" << s.name << "'" << std::endl;
}
