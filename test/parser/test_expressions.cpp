/**
 * @file   test_utility.cpp
 * @date   19.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <string>

#define BOOST_TEST_MODULE test_expressions
#include <boost/test/minimal.hpp>
#include <iostream>
#include <mw/test/parser/expressions.hpp>
#include <type_traits>

namespace x3 = boost::spirit::x3;


int test_main (int, char**)
{
	std::string s;

	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;


	iterator beg {s.begin()};
	iterator itr {s.begin()};
	iterator end {s.begin()};



	using mw::test::parser::equality;
	using mw::test::parser::comparison;
	using mw::test::parser::predicate;
	using mw::test::parser::skipper;

	using namespace mw::test::ast;

	std::string parsed;

	auto p = [&](auto rule, auto & buf)
		{
			using t = std::remove_reference_t<decltype(buf)>;
			buf = t();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end() );
			return x3::phrase_parse(itr, end, rule, skipper, buf);
		};


	mw::test::ast::equality e;

	s = "boost::variant<int, double> == 42";
	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	BOOST_CHECK(e.lhs.to_string() == "boost::variant<int, double> ");
	BOOST_CHECK(e.rhs.to_string() == "42");
	BOOST_CHECK(!e.tolerance);
	BOOST_CHECK(e.type == absolute);

	s = "x != 3.44 +/- 10%";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(e.inverted);
	BOOST_CHECK(e.lhs.to_string() == "x ");
	BOOST_CHECK(e.rhs.to_string() == "3.44 ");
	BOOST_REQUIRE(e.tolerance);
	BOOST_CHECK(e.tolerance->to_string() == "10");
	BOOST_CHECK(e.type == percent);

	s = "decltype(dings)::max == bla<23>::value +/- tolerance~";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	BOOST_CHECK(e.lhs.to_string() == "decltype(dings)::max ");
	BOOST_CHECK(e.rhs.to_string() == "bla<23>::value ");
	BOOST_REQUIRE(e.tolerance);
	BOOST_CHECK(e.tolerance->to_string() == "tolerance");
	BOOST_CHECK(e.type == relative);

	s = "x == y % z";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	BOOST_CHECK(e.lhs.to_string() == "x ");
	BOOST_CHECK(e.rhs.to_string() == "y % z");
	BOOST_CHECK(!e.tolerance);



	mw::test::ast::comparison c;


	s = "boost::variant<int, double>(23) >= 12";

	BOOST_CHECK(p(comparison, c));

	BOOST_CHECK(c.oper == greater_equal);
	BOOST_CHECK(c.lhs.to_string() == "boost::variant<int, double>(23) ");
	BOOST_CHECK(c.rhs.to_string() == "12");



	mw::test::ast::predicate pr;

	s = "(x<123>,y ) ";
	BOOST_CHECK(p(mw::test::parser::arg_list, pr.arg_list));
	BOOST_REQUIRE(pr.arg_list.data.size() == 2);

	BOOST_CHECK(pr.arg_list.data[0] == "x<123>");
	BOOST_CHECK(pr.arg_list.data[1] == "y");


	s = "std::less<int>()(x,y )";

	BOOST_CHECK(p(predicate, pr));

	BOOST_CHECK(pr.name.to_string() == "std::less<int>()");
	BOOST_REQUIRE(pr.arg_list.data.size() == 2);

	BOOST_CHECK(pr.arg_list.data[0] == "x");
	BOOST_CHECK(pr.arg_list.data[1] == "y");


	return 0;
}


