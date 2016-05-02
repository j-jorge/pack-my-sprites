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
#ifndef __PMS_GRAMMAR_HPP__
#define __PMS_GRAMMAR_HPP__

#include <boost/spirit/include/classic.hpp>

namespace pms
{
  class grammar:
    public boost::spirit::classic::grammar<grammar>
  {
  public:
    static const int id_file = 10;
    static const int id_sprite_sheet = 15;
    static const int id_margin = 17;
    static const int id_image_declaration = 20;
    static const int id_sprite_declaration = 40;
    static const int id_layer_reference = 45;
    static const int id_properties = 45;
    static const int id_string = 50;
    static const int id_glob = 55;
    static const int id_exclude = 57;
    static const int id_identifier = 90;

  public:
    template<typename ScannerT>
    class definition
    {
    private:
      class error_report_parser
      {
      public:
        typedef boost::spirit::classic::nil_t result_t;

      public:
        error_report_parser();
        error_report_parser( const std::string msg );

        template<typename LocalScanner>
        int operator()(const LocalScanner& scan, result_t& /*result*/) const;

      private:
        std::string m_msg;

      };

      class char_error_report_parser:
        public error_report_parser
      {
      public:
        char_error_report_parser();
        char_error_report_parser( char c );
      };

      typedef
      boost::spirit::classic::functor_parser<error_report_parser>
      generic_error;
      typedef
      boost::spirit::classic::functor_parser<char_error_report_parser>
      char_error;

    public:
      definition( const grammar& /*self*/ );

      const boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_file> >& start() const
      {
        return m_file;
      }

    private:
      void initialize_error_parsers();

    private:
      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_file> > m_file;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_sprite_sheet> > m_sprite_sheet;

      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_margin> > m_margin;

      boost::spirit::classic::rule< ScannerT > m_order;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_image_declaration> >
      m_image_declaration;

      boost::spirit::classic::rule<ScannerT> m_sprite_description;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_sprite_declaration> >
      m_sprite_declaration;

      boost::spirit::classic::rule<ScannerT> m_mask;

      boost::spirit::classic::rule<ScannerT> m_layer_list;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_layer_reference> >
        m_layer_reference;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_properties> >
        m_properties;

      boost::spirit::classic::rule<ScannerT> m_sprite_size;

      boost::spirit::classic::rule
      < ScannerT,
        boost::spirit::classic::parser_tag<id_identifier> > m_identifier;

      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_string> > m_string;

      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_glob> > m_glob;

      boost::spirit::classic::rule
      < ScannerT, boost::spirit::classic::parser_tag<id_exclude> > m_exclude;

      generic_error m_error_sprite_sheet;
      generic_error m_error_autosize;
      generic_error m_error_identifier;
      generic_error m_error_string;
      generic_error m_error_not_terminated_string;
      generic_error m_error_size;
      char_error m_error_dot;
      char_error m_error_semicolon;

    };
  };
}

#include "impl/grammar.tpp"

#endif
