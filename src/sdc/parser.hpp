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
 * \brief The parser for the spritedesc files.
 * \author Julien Jorge
 */
#ifndef __SDC_PARSER_HPP__
#define __SDC_PARSER_HPP__

#include "spritedesc.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <istream>

namespace sdc
{
  class xcf_map;

  /**
   * \brief The parser for the .spritedesc files.
   * \author Julien Jorge
   */
  class parser
  {
  public:
    typedef boost::spirit::classic::position_iterator<const char*> iterator;
    typedef
      boost::spirit::classic::node_iter_data_factory<iterator> node_factory;
    typedef
      boost::spirit::classic::tree_match<iterator, node_factory> tree_match;
    typedef tree_match::node_t tree_node;

  private:
    typedef boost::spirit::classic::scanner<iterator> scanner;

  public:
    bool run( xcf_map& xcf, std::list<spritedesc>& desc, std::istream& in );
    bool run
      ( xcf_map& xcf, std::list<spritedesc>& desc, const char* file_data,
        unsigned int file_size );

  private:
    void scan_tree
    ( xcf_map& xcf, std::list<spritedesc>& desc, const tree_node& node ) const;

  }; // class parser
} // namespace sdc

#endif // __SDC_PARSER_HPP__

