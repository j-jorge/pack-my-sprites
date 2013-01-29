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
 * \brief Base class for node parsers.
 * \author Julien Jorge
 */
#ifndef __SDC_NODE_PARSER_HPP__
#define __SDC_NODE_PARSER_HPP__

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <string>

namespace sdc
{
  /**
   * \brief Base class for node parsers.
   * \author Julien Jorge
   */
  class node_parser
  {
  public:
    /** \brief The type of an iterator on the input data. */
    typedef
    boost::spirit::classic::position_iterator<const char*> data_iterator;

    typedef
    boost::spirit::classic::node_iter_data_factory<data_iterator>
    node_factory;
    typedef
    boost::spirit::classic::tree_match<data_iterator, node_factory>
    tree_match;

    /** \brief The type of a node of the tree built by the compiler. */
    typedef tree_match::node_t tree_node;

    typedef std::vector<tree_node>::const_iterator node_children_iterator;

  protected:
    void error
    ( data_iterator first, data_iterator last, const std::string& msg ) const;
    
  }; // class node_parser
} // namespace sdc

#endif // __SDC_NODE_PARSER_HPP__
