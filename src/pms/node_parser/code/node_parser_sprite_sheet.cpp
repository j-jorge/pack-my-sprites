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
#include "node_parser/node_parser_sprite_sheet.hpp"

#include "node_parser/node_parser_xcf_entry.hpp"
#include "node_parser/node_parser_sprite_declaration.hpp"

#include "grammar.hpp"

#include "spritedesc.hpp"

void pms::node_parser_sprite_sheet::parse_node
( image_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  CLAW_PRECOND( node.children.size() > 4 );

  parse_name( desc, node.children[0] );
  parse_width( desc, node.children[1] );
  parse_height( desc, node.children[2] );
  
  std::size_t i(3);

  if ( parse_margin( desc, node.children[i] ) )
    ++i;

  if ( parse_order( desc, node.children[i] ) )
    ++i;

  while ( (i != node.children.size())
          && ( node.children[i].value.id() == grammar::id_xcf_declaration ) )
    {
      node_parser_xcf_entry call;
      call.parse_node( xcf, desc, node.children[i] );
      ++i;
    }

  while ( i != node.children.size() )
    {
      if ( node.children[i].value.id() == grammar::id_sprite_declaration )
        {
          node_parser_sprite_declaration call;
          call.parse_node( xcf, desc, node.children[i] );
        }
      else
        {
          const std::string value
            ( node.children[i].value.begin(), node.children[i].value.end() );

          std::cerr << "Can't parse '" << value << "'.";
        }

      ++i;
    }

  check_xcf_usage( desc );
}

void pms::node_parser_sprite_sheet::parse_name
( spritedesc& desc, const tree_node& node ) const
{
  desc.output_name = std::string( node.value.begin(), node.value.end() );
}

void pms::node_parser_sprite_sheet::parse_width
( spritedesc& desc, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> desc.width;
}

void pms::node_parser_sprite_sheet::parse_height
( spritedesc& desc, const tree_node& node ) const
{
  std::istringstream iss( std::string( node.value.begin(), node.value.end() ) );
  iss >> desc.height;
}

bool pms::node_parser_sprite_sheet::parse_margin
( spritedesc& desc, const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_margin )
    {
      std::istringstream iss
        ( std::string( node.value.begin(), node.value.end() ) );
      iss >> desc.margin;
      return true;
    }
  else
    {
      desc.margin = 1;
      return false;
    }
}

bool pms::node_parser_sprite_sheet::parse_order
( spritedesc& desc, const tree_node& node ) const
{
  desc.order = decreasing_area;

  if ( node.value.id() == grammar::id_string )
    {
      const std::string order_string( node.value.begin(), node.value.end() );

      if ( order_string == "none" )
        desc.order = declaration_order;
      else if ( order_string == "area" )
        desc.order = decreasing_area;
      else if ( order_string == "height" )
        desc.order = decreasing_height;
      else
        std::cerr << "Unknown order type '" << order_string << "'."
                  << std::endl;

      return true;
    }
  else
    return false;
}

void pms::node_parser_sprite_sheet::check_xcf_usage
( const spritedesc& desc ) const
{
  spritedesc::id_to_file_map xcf_files = desc.xcf;
  
  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    xcf_files.erase( it->xcf_id );

  for ( spritedesc::id_to_file_map::const_iterator it = xcf_files.begin();
        it != xcf_files.end(); ++it )
    std::cerr << "warning: XCF file '" << it->second << "' identified as '"
              << it->first << "' is never used in sprite sheet '"
              << desc.output_name << "'." << std::endl;
}
