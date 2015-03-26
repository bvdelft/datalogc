#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <complex>

#include "structures.h"

BOOST_FUSION_ADAPT_STRUCT (
  client::atom, 
    (std::string, predicate) 
    (std::vector<std::string>, arguments)
)
BOOST_FUSION_ADAPT_STRUCT (
  client::clause, 
    (client::atom, head) 
    (std::vector<client::atom>, body)
)

namespace client
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  template <typename Iterator>
  struct clause_parser : qi::grammar<Iterator, std::vector<clause>(), ascii::space_type>
  {
    clause_parser() : clause_parser::base_type(start)
    {
      using qi::lexeme;
      using ascii::char_;

      argument %= +(char_("a-zA-Z"));

      arglist %= argument >> *(',' >> argument);

      aatom %= argument
        >> '('
        >> -arglist
        >> ')';

      body %= aatom >> *(',' >> aatom);

      cclause %= aatom
        >> -(qi::lit(":-") >> body)
        >> '.';

      start %= +(cclause);

    }
    qi::rule<Iterator, std::string()> argument;
    qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> arglist;
    qi::rule<Iterator, atom(), ascii::space_type> aatom;
    qi::rule<Iterator, std::vector<atom>(), ascii::space_type> body;
    qi::rule<Iterator, clause(), ascii::space_type> cclause;
    qi::rule<Iterator, std::vector<clause>(), ascii::space_type> start;
  };

  bool parse(std::string str, std::vector<clause>& clauses)
  {
    using namespace std;
    using boost::spirit::ascii::space;
    typedef string::const_iterator iterator_type;
    typedef client::clause_parser<iterator_type> clause_parser;
    clause_parser g;

    string::const_iterator iter = str.begin();
    string::const_iterator end = str.end();
    return phrase_parse(iter, end, g, space, clauses);
  }
}
