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
#include "pms/serialization/spritedesc/parser.hpp"

#include "pms/serialization/spritedesc/grammar.hpp"
#include "pms/serialization/spritedesc/node_parser_file.hpp"

#include <sstream>

bool pms::serialization::spritedesc::parser::run
( layout::atlas& atlas, std::istream& in )
{
  std::stringstream file_data;
  file_data << in.rdbuf();
  
  return run( atlas, file_data.str().c_str(), file_data.str().size() );
}

bool pms::serialization::spritedesc::parser::run
( layout::atlas& atlas, const char* file_data, unsigned int file_size )
{
  bool ok;

  boost::spirit::classic::tree_parse_info<iterator, node_factory> info;
  grammar grammar;
  iterator begin(file_data, file_data + file_size, "script"), end;

  info = boost::spirit::classic::ast_parse<node_factory>
    ( begin, end, grammar,
      boost::spirit::classic::comment_p("/*", "*/")
      | boost::spirit::classic::space_p );

  ok = info.match;

  scan_tree( atlas, info.trees[0] );

  return ok;
}

void pms::serialization::spritedesc::parser::scan_tree
( layout::atlas& atlas, const tree_node& node ) const
{
  node_parser_file file;

  file.parse_node( atlas, node );
}
