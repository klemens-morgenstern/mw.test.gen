/**
 * @file   expression.hpp
 * @date   07.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_EXPRESSIONS_HPP_
#define MW_TEST_DATA_EXPRESSIONS_HPP_

#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/code.hpp>
#include <mw/test/ast/expressions.hpp>


BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::equality,
	(mw::test::ast::code, lhs)
	(bool, inverted)
	(mw::test::ast::code, rhs)
	(boost::optional<mw::test::ast::code>, tolerance)
	(mw::test::ast::relativity_t, type)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::comparison,
	(mw::test::ast::code, lhs)
	(mw::test::ast::compare_op, oper)
	(mw::test::ast::code, rhs)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::predicate,
	(mw::test::ast::code, name)
	(mw::test::ast::code_list, arg_list)
);


namespace mw
{
namespace test
{
namespace parsers
{

struct relativity_t : x3::symbols<ast::relativity_t>
{
	relativity_t()
	{
		using e = ast::relativity_t;
		add ("%", e::percent)
			("~", e::relative)
			;
	}
} relativity;

struct equal_sym_t : x3::symbols<bool>
{
	equal_sym_t()
	{
		add	("==", 	false)
			("!=", 	true)
			;
	}
} equal_sym;


struct compare_op_t : x3::symbols<mw::test::ast::compare_op>
{
	compare_op_t()
	{
		using t = mw::test::ast::compare_op;
		add (">=", t::greater_equal)
			(">" , t::greater)
			("<=", t::lesser_equal)
			("<" , t::lesser)
			;

	}
} compare_op;


x3::rule<class arg_list, ast::code_list> const arg_list;
auto const arg_list_def = '(' >> code_list >> ')' >> eoi;

BOOST_SPIRIT_DEFINE(arg_list);

x3::rule<class code_no_equal, ast::code> const code_no_equal;
auto const code_no_equal_def =
		eps[code::set_beg] >>
			omit[ lexeme[*((!(equal_sym | "+/-" )  >> code_chunk_step)| (relativity >> code_chunk_step))]]
			  >> eps[code::set_end];

BOOST_SPIRIT_DEFINE(code_no_equal);


x3::rule<class code_no_comparison, ast::code> const code_no_comparison;
auto const code_no_comparison_def =
		eps[code::set_beg] >>
			omit[ lexeme[+code_chunk_step_no_ops]]
			  >> eps[code::set_end];

BOOST_SPIRIT_DEFINE(code_no_comparison);


x3::rule<class code_no_relative, ast::code> const code_no_relative;
auto const code_no_relative_def =
		eps[code::set_beg] >>
			omit[ lexeme[+(!relativity >> code_chunk_step)]]
			  >> eps[code::set_end];

BOOST_SPIRIT_DEFINE(code_no_relative);



x3::rule<class code_no_arg_list, ast::code> const code_no_arg_list;
auto const code_no_arg_list_def =
		eps[code::set_beg] >>
			omit[ lexeme[*(!skip[arg_list] >> code_chunk_step) ]]
			>> eps[code::set_end];

BOOST_SPIRIT_DEFINE(code_no_arg_list);

x3::rule<class equality, ast::equality> const equality;
auto const equality_def =
		code_no_equal >>
		equal_sym >>
		code_no_equal >>
		-("+/-" >> code_no_relative) >>
		-relativity >>
		eoi;

BOOST_SPIRIT_DEFINE(equality);


x3::rule<class comparison, ast::comparison> const comparison;
auto const comparison_def = code_no_comparison >> compare_op >> code_no_comparison ;

BOOST_SPIRIT_DEFINE(comparison);

x3::rule<class predicate, ast::predicate> const predicate;
auto const predicate_def = code_no_arg_list >> arg_list;

BOOST_SPIRIT_DEFINE(predicate);

}
}
}



#endif /* MW_TEST_AST_EXPRESSIONS_HPP_ */
