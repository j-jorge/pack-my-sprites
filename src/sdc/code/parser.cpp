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
/**
 * \file
 * \brief Implementation of the sdc::parser class.
 * \author Julien Jorge
 */
#include "parser.hpp"

#include "spritedesc.hpp"
#include "grammar.hpp"

#include "node_parser/node_parser_file.hpp"

#include <fstream>
#include <sstream>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the description of the sprites.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param path The path of the file to read.
 */
bool sdc::parser::run
( xcf_map& xcf, std::list<spritedesc>& desc, std::istream& in )
{
  std::stringstream file_data;
  file_data << in.rdbuf();
  
  return run( xcf, desc, file_data.str().c_str(), file_data.str().size() );
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse the description of the sprites.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param file_data The content of the file to compile.
 * \param file_size Size of the \a file_data buffer.
 */
bool sdc::parser::run
( xcf_map& xcf, std::list<spritedesc>& desc, const char* file_data,
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
} // model_compiler::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the tree of the file.
 * \param xcf The map associating the name of the xcf with their descriptions.
 * \param desc The result of the parsing.
 * \param node The root of the tree to compile.
 */
void sdc::parser::scan_tree
( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const
{
  node_parser_file file;

  file.parse_node( xcf, desc, node );
} // parser::parse_tree()
