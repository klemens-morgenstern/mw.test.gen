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
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/id.hpp>

#include <mw/test/ast/tpl_arg.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
		mw::test::ast::tpl_arg,
		(std::string, name)
		(std::string, def_arg)
);

BOOST_FUSION_ADAPT_STRUCT(
		mw::test::ast::obj_id,
		(mw::test::ast::code::iterator, location)
		(std::string, name)
		(std::vector<std::string>, tpl_args)
);


namespace mw
{
namespace test
{
namespace parser
{

///Escaped template for []
/**@code{.ebnf}
 * <tpl_square_par> ::= '[' tpl_par_step ']';
 * @endcode
 */
x3::rule<class tpl_square_par, std::string> const  tpl_square_par;

///Escaped template for ()
/**@code{.ebnf}
 * <tpl_round_par> ::= '(' tpl_par_step ')';
 * @endcode
 */
x3::rule<class tpl_round_par , std::string> const  tpl_round_par ;

///Escaped template for ()
/**@code{.ebnf}
 * <tpl_curly_par> ::= '{' tpl_par_step '}';
 * @endcode
 */
x3::rule<class tpl_curly_par , std::string> const  tpl_curly_par ;

///Escaped template for <>
/**@code{.ebnf}
 * <tpl_pointy_par> ::= '<' tpl_par_step '>';
 * @endcode
 */
x3::rule<class tpl_pointy_par, std::string> const  tpl_pointy_par;

/// Parser rule for an entry in the template-parameter-list
/** @code{.ebnf}
 * <tpl_par> ::= <tpl_par_step>* ;
 *  @endcode
 */
x3::rule<class tpl_par, 	 std::string> const tpl_par;

/// Parser rule for an element of a template parameter list, basically a token
/**@code{.ebnf}
 * <tpl_par> ::= -'[[]{}()<>]' |
 * 				<tpl_square_par> | <tpl_round_par> |
 * 				<tpl_curly_par>  | <tpl_pointy_par>|
 * 				<quoted_string>	 | <squoted_string> ;
 * @endcode
 */
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

///A template argument
/**@code{.ebnf}
 * <tpl_arg> ::= <id> | (<id> '=' <tpl_par> );
 * @endcode
 *
 */
x3::rule<class tpl_arg, ast::tpl_arg> tpl_arg;

auto const tpl_arg_def = id >> -('=' >> tpl_par);



auto push_back = [](auto &ctx)
		{
			_val(ctx).push_back(_attr(ctx));
		};

///Rule for a template declaration list
/**@code{.ebnf}
 * <tpl_decl> ::= '<' -(<id> (',' <id> )* ) '>' ;
 * @endcode
 */
x3::rule<class tpl_decl, std::vector<ast::tpl_arg>> const tpl_decl;

auto const tpl_decl_def 	= '<' >> -(tpl_arg % ',') >> '>';


///Rule for the template parameter list, i.e. definition
/**@code{.ebnf}
 * <tpl_par_list> ::= '<' <tpl_par> (',' <tpl_par> )* '>';
 * @endcode
 */
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



x3::rule<class obj_id, ast::obj_id> obj_id;

auto const obj_id_def =  code_location >> id >> -tpl_par_list;

BOOST_SPIRIT_DEFINE(obj_id);

}
}
}



#endif /* MW_TEST_PARSER_TEMPLATE_HPP_ */
