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
#include "parser.hpp"

#include "spritedesc.hpp"
#include "grammar.hpp"

#include "node_parser/node_parser_file.hpp"

#include <fstream>
#include <sstream>

bool pms::parser::run
( xcf_map& xcf, spritedesc& desc, std::istream& in )
{
  std::stringstream file_data;
  file_data << in.rdbuf();
  
  return run( xcf, desc, file_data.str().c_str(), file_data.str().size() );
}

bool pms::parser::run
( xcf_map& xcf, spritedesc& desc, const char* file_data,
  unsigned int file_size )
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

  scan_tree( xcf, desc, info.trees[0] );

  return ok;
}

void pms::parser::scan_tree
( xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  node_parser_file file;

  file.parse_node( xcf, desc, node );
}
