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

#include "pms/layout/description.hpp"

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>

#include <istream>

namespace pms
{
  namespace layout
  {
    class sprite_sheet;
  }

  namespace serialization
  {
    namespace spritedesc
    {
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
        bool run( layout::sprite_sheet& sheet, std::istream& in );
        bool run
        ( layout::sprite_sheet& sheet, const char* file_data,
          unsigned int file_size );

      private:
        void scan_tree
        ( layout::sprite_sheet& sheet, const tree_node& node ) const;
      };
    }
  }
}