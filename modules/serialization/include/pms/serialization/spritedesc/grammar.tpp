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
#include <boost/spirit/include/classic_ast.hpp>

template <typename ScannerT>
pms::serialization::spritedesc::grammar::definition<ScannerT>::
error_report_parser::error_report_parser()
{

}

template <typename ScannerT>
pms::serialization::spritedesc::grammar::definition<ScannerT>::
error_report_parser::error_report_parser( const std::string msg )
  : m_msg(msg)
{

}

template <typename ScannerT>
template <typename LocalScanner>
int
pms::serialization::spritedesc::grammar::definition<ScannerT>::
error_report_parser::operator()
  (const LocalScanner& scan, result_t& /*result*/) const
{
  boost::spirit::classic::file_position fpos = scan.first.get_position();

  std::cerr << fpos.file << ": " << fpos.line << ": "
            << fpos.column << ": " << m_msg << std::endl;

  return -1;
}

template <typename ScannerT>
pms::serialization::spritedesc::grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser()
{

}

template <typename ScannerT>
pms::serialization::spritedesc::grammar::definition<ScannerT>::
char_error_report_parser::char_error_report_parser( char c )
  : error_report_parser( std::string("Missing character '") + c + "'." )
{

}





template<typename ScannerT>
pms::serialization::spritedesc::grammar::definition<ScannerT>::definition
( const grammar& self )
{
  initialize_error_parsers();

  m_file = m_atlas;

  m_atlas =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("atlas") ]
    >> ( m_string | m_error_atlas )
    >> ( ( boost::spirit::classic::uint_p
           >> boost::spirit::classic::no_node_d
           [ boost::spirit::classic::ch_p('x') ]
           >> boost::spirit::classic::uint_p )
         | m_error_size )
    >> !m_margin
    >> !m_order
    >> *m_image_declaration >> *m_sprite_description;

  m_margin =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("margin") ]
    >> boost::spirit::classic::uint_p
    ;

  m_order =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("order") ]
    >> (m_string | m_error_string)
    ;

  m_image_declaration =
    m_identifier >> (m_string | m_error_string);

  m_sprite_description = m_sprite_declaration;

  m_sprite_declaration =
    !m_properties
    >> m_string
    >> ( m_sprite_size | m_error_autosize )
    >> boost::spirit::classic::no_node_d
       [ boost::spirit::classic::strlit<>("with") ]
    >> m_identifier
    >> m_layer_list
    >> !m_mask
    >> ( boost::spirit::classic::no_node_d
         [ boost::spirit::classic::ch_p(';') ] | m_error_semicolon )
    ;

  m_mask =
    boost::spirit::classic::no_node_d
    [ boost::spirit::classic::strlit<>("mask") ]
    >> m_layer_list;
    ;

  m_layer_list =
    *m_layer_reference;

  m_layer_reference =
    ( !m_properties >> (m_string | m_glob) )
    | m_exclude;

  m_properties =
    boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('[') ]
    >> *m_identifier
    >> boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p(']') ]
    ;

  m_glob =
    boost::spirit::classic::strlit<>("glob")
    >> m_string;
    ;

  m_exclude = 
    boost::spirit::classic::strlit<>("exclude")
    >> (m_string | m_glob);

  m_sprite_size =
    ( m_identifier | m_string )
    >> !( boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('*') ]
          >> boost::spirit::classic::real_p );

  m_identifier =
    boost::spirit::classic::no_node_d[ *boost::spirit::classic::blank_p ]
    >>    boost::spirit::classic::token_node_d
    [ boost::spirit::classic::lexeme_d[ (boost::spirit::classic::alpha_p | '_')
                               >> *(boost::spirit::classic::alnum_p | '_') ]
      ];

  m_string =
    boost::spirit::classic::lexeme_d
    [ boost::spirit::classic::no_node_d[ boost::spirit::classic::ch_p('"') ]
      >> boost::spirit::classic::token_node_d
      [ *(boost::spirit::classic::strlit<>("\\\"")
          | (boost::spirit::classic::anychar_p - '"') ) ]
      >> boost::spirit::classic::no_node_d
      [ ( '"' | m_error_not_terminated_string ) ]
    ];
}

template<typename ScannerT>
void pms::serialization::spritedesc::grammar::definition<ScannerT>::
initialize_error_parsers()
{
  m_error_autosize =
    error_report_parser( "Size expected, like: autosize * 0.5." );
  m_error_identifier = error_report_parser( "Identifier expected." );
  m_error_string = error_report_parser( "String expected." );
  m_error_not_terminated_string =
    error_report_parser( "Not terminated string." );

  m_error_atlas = error_report_parser( "atlas expected" );
  m_error_size = error_report_parser( "Size expected (width x height)." );

  m_error_semicolon = char_error_report_parser( ';' );
}
