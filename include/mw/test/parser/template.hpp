/**
 * @file   template.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_TEMPLATE_HPP_
#define MW_TEST_PARSER_TEMPLATE_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/code.hpp>

namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class tpl_square_par> const  tpl_square_par("tpl_square_par");
x3::rule<class tpl_round_par>  const  tpl_round_par ("tpl_round_par");
x3::rule<class tpl_curly_par>  const  tpl_curly_par ("tpl_curly_par");
x3::rule<class tpl_curly_par>  const  quote_par	    ("quote_par");

x3::rule<class tpl_par> const tpl_par("tpl_par");


auto const tpl_square_par_def =
		"[" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> "]"
		;

auto const tpl_round_par_def =
		"(" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> ")"
		;

auto const tpl_curly_par_def =
		"{" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> "}"
		;

auto const quote_par_def =
		('"' >> *(!char_('"') >>  ("\\\"" | char_ ) ) >> '"')
		| ('\'' >> char_ >> '\'')
		;

auto const tpl_par_def =
		 + (!(char_("[](){},") | string("...")) >> (+char_ >> quote_par))
		 >> *(
		  tpl_square_par
		| tpl_round_par
		| tpl_curly_par
		| no_skip[+(!(char_(",") | string("...")) >> char_)]);





x3::rule<class template_decl> const template_decl("template_params");

auto const template_decl_def =
		'<' >> (id % ',') >> '>';



x3::rule<class tuple> const tuple("tuple");
auto const tuple_def = "(" >> (tpl_par % ',') >> ')';

x3::rule<class range> const range("range");
auto const range_def = '[' >> (tpl_par | tuple) >> "..." >> (tpl_par | tuple) >> ']';

x3::rule<class par_list> const par_list("par_list");
auto const par_list_def = "[" >> (tpl_par % ",") >> "]";

x3::rule<class tpl_par_list> const tpl_par_list("tpl_par_list");
auto const tpl_par_list_def = '<' >> (tpl_par | par_list_def | range )% ',' >> '>'	;


}
}
}



#endif /* MW_TEST_PARSER_TEMPLATE_HPP_ */
