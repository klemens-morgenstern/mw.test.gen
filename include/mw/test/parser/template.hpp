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
#include <mw/test/parser/utility.hpp>
//#include <mw/test/parser/code.hpp>
#include <mw/test/parser/id.hpp>

#include <mw/test/data/tpl_arg.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
		mw::test::data::tpl_arg,
		(std::string, name)
		(std::string, def_arg)
);

namespace mw
{
namespace test
{
namespace parser
{


x3::rule<class tpl_square_par, std::string> const  tpl_square_par;
x3::rule<class tpl_round_par , std::string> const  tpl_round_par ;
x3::rule<class tpl_curly_par , std::string> const  tpl_curly_par ;
x3::rule<class tpl_pointy_par, std::string> const  tpl_pointy_par;

x3::rule<class tpl_par, 	 std::string> const tpl_par;
x3::rule<class tpl_par_step, std::string> const tpl_par_step;

auto const tpl_par_step_def =
			((!char_("[]{}()<>") >> char_)
			| tpl_square_par
			| tpl_round_par
			| tpl_curly_par
			| tpl_pointy_par
			|  quoted_string
			| squoted_string);


auto const tpl_square_par_def = char_("[") >> *tpl_par_step >> char_("]");
auto const tpl_round_par_def  = char_("(") >> *tpl_par_step >> char_(")");
auto const tpl_curly_par_def  = char_("{") >> *tpl_par_step >> char_("}");
auto const tpl_pointy_par_def = char_("<") >> *tpl_par_step >> char_(">");

auto const tpl_par_def = lexeme[*(!lit(',') >> tpl_par_step)];


x3::rule<class tpl_arg, data::tpl_arg> tpl_arg;

auto const tpl_arg_def = id >> -('=' >> tpl_par);



auto push_back = [](auto &ctx)
		{
			_val(ctx).push_back(_attr(ctx));
		};
///Rule for parameter list
/**@code{.ebnf}
 * <tpl_decl> ::= '<' -(<id> (',' <id> )* ) '>' ;
 * @endcode
 *
 */
x3::rule<class tpl_decl, std::vector<data::tpl_arg>> const tpl_decl;

auto const tpl_decl_def 	= '<' >> -(tpl_arg % ',') >> '>';

x3::rule<class tpl_par_list, std::vector<std::string>> const tpl_par_list;

auto const tpl_par_list_def = '<' >> tpl_par % ',' >> '>';

BOOST_SPIRIT_DEFINE(tpl_par_step);
BOOST_SPIRIT_DEFINE(tpl_arg);
BOOST_SPIRIT_DEFINE(tpl_square_par);
BOOST_SPIRIT_DEFINE(tpl_round_par);
BOOST_SPIRIT_DEFINE(tpl_curly_par);
BOOST_SPIRIT_DEFINE(tpl_pointy_par);
BOOST_SPIRIT_DEFINE(tpl_par);
BOOST_SPIRIT_DEFINE(tpl_decl);
BOOST_SPIRIT_DEFINE(tpl_par_list);

}
}
}



#endif /* MW_TEST_PARSER_TEMPLATE_HPP_ */
