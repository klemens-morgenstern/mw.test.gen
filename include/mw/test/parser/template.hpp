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
		  +(
		  tpl_square_par
		| tpl_round_par
		| tpl_curly_par
		| quote_par
		| no_skip[+(!(char_(",")) >> char_)]);

x3::rule<class object_content> const object_content("object_content");

auto const object_content_def =
		eps;// =



x3::rule<class template_decl> const template_decl("template_params");

auto const template_decl_def =
		'<' >> (id % ',') >> '>';

x3::rule<class templ_param> const templ_param("templ_param");

auto const templ_param_def = '<' >> tpl_par % ',' >> '>'	;

}
}
}



#endif /* MW_TEST_PARSER_TEMPLATE_HPP_ */
