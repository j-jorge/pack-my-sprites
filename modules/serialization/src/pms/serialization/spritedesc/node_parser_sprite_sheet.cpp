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
#include "pms/serialization/spritedesc/node_parser_sprite_sheet.hpp"

#include "pms/serialization/spritedesc/grammar.hpp"
#include "pms/serialization/spritedesc/node_parser_image_entry.hpp"
#include "pms/serialization/spritedesc/node_parser_sprite_declaration.hpp"

#include "pms/layout/description.hpp"
#include "pms/layout/sprite_sheet.hpp"

void pms::serialization::spritedesc::node_parser_sprite_sheet::parse_node
( layout::sprite_sheet& sheet, const tree_node& node ) const
{
  CLAW_PRECOND( node.children.size() > 4 );

  parse_name( sheet, node.children[0] );
  parse_width( sheet, node.children[1] );
  parse_height( sheet, node.children[2] );
  
  std::size_t i(3);

  if ( parse_margin( sheet, node.children[i] ) )
    ++i;

  // skip order
  if ( node.children[i].value.id() == grammar::id_string )
    ++i;

  layout::description desc;
  
  while ( (i != node.children.size())
          && ( node.children[i].value.id() == grammar::id_image_declaration ) )
    {
      node_parser_image_entry call;
      call.parse_node( sheet.image, desc, node.children[i] );
      ++i;
    }

  while ( i != node.children.size() )
    {
      if ( node.children[i].value.id() == grammar::id_sprite_declaration )
        {
          node_parser_sprite_declaration call;
          call.parse_node( sheet.image, desc, node.children[i] );
        }
      else
        {
          const std::string value
            ( node.children[i].value.begin(), node.children[i].value.end() );

          std::cerr << "Can't parse '" << value << "'.";
        }

      ++i;
    }

  check_image_usage( sheet.output_name, desc );

  sheet.pages.push_back( desc );
}

void pms::serialization::spritedesc::node_parser_sprite_sheet::parse_name
( layout::sprite_sheet& sheet, const tree_node& node ) const
{
  sheet.output_name = std::string( node.value.begin(), node.value.end() );
}

void pms::serialization::spritedesc::node_parser_sprite_sheet::parse_width
( layout::sprite_sheet& sheet, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> sheet.width;
}

void pms::serialization::spritedesc::node_parser_sprite_sheet::parse_height
( layout::sprite_sheet& sheet, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> sheet.height;
}

bool pms::serialization::spritedesc::node_parser_sprite_sheet::parse_margin
( layout::sprite_sheet& sheet, const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_margin )
    {
      std::istringstream iss
        ( std::string( node.value.begin(), node.value.end() ) );
      iss >> sheet.margin;
      return true;
    }
  else
    {
      sheet.margin = 1;
      return false;
    }
}

void pms::serialization::spritedesc::node_parser_sprite_sheet::check_image_usage
( const std::string& output_name, const layout::description& desc ) const
{
  layout::description::id_to_file_map images = desc.images;
  
  for ( layout::description::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    images.erase( it->image_id );

  for ( layout::description::id_to_file_map::const_iterator it = images.begin();
        it != images.end(); ++it )
    std::cerr << "warning: Image file '" << it->second << "' identified as '"
              << it->first << "' is never used in sprite sheet '"
              << output_name << "'." << std::endl;
}
