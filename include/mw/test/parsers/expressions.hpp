/**
 * @file   expression.hpp
 * @date   07.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_EXPRESSIONS_HPP_
#define MW_TEST_PARSER_EXPRESSIONS_HPP_

#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/code.hpp>
#include <mw/test/data/expressions.hpp>


BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::equality,
	(mw::test::data::code, lhs)
	(bool, inverted)
	(mw::test::data::code, rhs)
	(boost::optional<mw::test::data::code>, tolerance)
	(mw::test::data::relativity_t, type)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::comparison,
	(mw::test::data::code, lhs)
	(mw::test::data::compare_op, oper)
	(mw::test::data::code, rhs)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::predicate,
	(mw::test::data::code, name)
	(mw::test::data::code_list, arg_list)
);


namespace mw
{
namespace test
{
namespace parsers
{

struct relativity_t : x3::symbols<data::relativity_t>
{
	relativity_t()
	{
		using e = data::relativity_t;
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


struct compare_op_t : x3::symbols<data::compare_op>
{
	compare_op_t()
	{
		using t = mw::test::data::compare_op;
		add (">=", t::greater_equal)
			(">" , t::greater)
			("<=", t::lesser_equal)
			("<" , t::lesser)
			;

	}
} compare_op;


x3::rule<class arg_list, data::code_list> const arg_list;
auto const arg_list_def = '(' >> code_list >> ')' >> eoi;

BOOST_SPIRIT_DEFINE(arg_list);

template<typename T, typename U>
inline void range_to_code_assign(T &, const U&) {}
inline void range_to_code_assign(data::code & val, const boost::iterator_range<iterator> & attr)
{
    val.content = std::string(attr.begin(), attr.end());
    val.begin = parser::instance().current_file().get_location(attr.begin());
    val.end   = parser::instance().current_file().get_location(attr.end());
}

auto range_to_code = [](auto &ctx) { range_to_code_assign(x3::_val(ctx), x3::_attr(ctx)); };


x3::rule<class code_no_equal, data::code> const code_no_equal;
auto const code_no_equal_def =
		raw[  lexeme[*((!( ";" | equal_sym | "+/-" )  >> code_chunk_step) | (relativity >> code_chunk_step))]
		         ][range_to_code];

BOOST_SPIRIT_DEFINE(code_no_equal);


x3::rule<class code_no_comparison, data::code> const code_no_comparison;
auto const code_no_comparison_def =
		raw[
			 lexeme[+(!lit(';') >> code_chunk_step_no_ops)]
			      ][range_to_code];

BOOST_SPIRIT_DEFINE(code_no_comparison);


x3::rule<class code_no_relative, data::code> const code_no_relative;
auto const code_no_relative_def =
		    raw[
			 lexeme[+(!(';' | relativity) >> code_chunk_step)]
			      ][range_to_code];

BOOST_SPIRIT_DEFINE(code_no_relative);



x3::rule<class code_no_arg_list, data::code> const code_no_arg_list;
auto const code_no_arg_list_def =
		        raw[
			 lexeme[*(!(skip[arg_list] | ';') >> code_chunk_step) ]
			      ][range_to_code];

BOOST_SPIRIT_DEFINE(code_no_arg_list);

x3::rule<class equality, data::equality> const equality;
auto const equality_def =
		code_no_equal >>
		equal_sym >>
		code_no_equal >>
		-("+/-" >> code_no_relative) >>
		-relativity;

BOOST_SPIRIT_DEFINE(equality);


x3::rule<class comparison, data::comparison> const comparison;
auto const comparison_def = code_no_comparison >> compare_op >> code_no_comparison ;
BOOST_SPIRIT_DEFINE(comparison);

x3::rule<class predicate, data::predicate> const predicate;
auto const predicate_def = code_no_arg_list >> arg_list;

BOOST_SPIRIT_DEFINE(predicate);


x3::rule<class expression, data::expression> const expression;
auto const expression_def =
           (equality   >> ";")  |
           (comparison >> ";")  |
           (predicate  >> ";")  |
           code_chunk ; //should be regarded as temporary, since things like (x == 42) || (x == 12) might be added in the future.

BOOST_SPIRIT_DEFINE(expression);

}
}
}



#endif /* MW_TEST_AST_EXPRESSIONS_HPP_ */
