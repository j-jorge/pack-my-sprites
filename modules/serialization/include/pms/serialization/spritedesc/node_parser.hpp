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
#pragma once

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <string>

namespace pms
{
  namespace serialization
  {
    namespace spritedesc
    {
      class node_parser
      {
      public:
        typedef
        boost::spirit::classic::position_iterator<const char*> data_iterator;

        typedef
        boost::spirit::classic::node_iter_data_factory<data_iterator>
        node_factory;
        typedef
        boost::spirit::classic::tree_match<data_iterator, node_factory>
        tree_match;

        typedef tree_match::node_t tree_node;

        typedef std::vector<tree_node>::const_iterator node_children_iterator;

      protected:
        void error
        ( data_iterator first, data_iterator last,
          const std::string& msg ) const;
    
      };
    }
  }
}
