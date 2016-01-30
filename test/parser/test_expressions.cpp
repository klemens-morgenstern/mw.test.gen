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
#include <mw/test/parsers/expressions.hpp>
#include <mw/test/parser.hpp>
#include <type_traits>

namespace x3 = boost::spirit::x3;


int test_main (int, char**)
{
    mw::test::parser parser;

     parser.include_stack.emplace(std::string(""));

     std::string &s = parser.include_stack.top().buffer;


     using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

     namespace x3 = boost::spirit::x3;

     auto &beg = parser.include_stack.top()._begin;
     auto itr  = parser.include_stack.top()._begin;
     auto &end = parser.include_stack.top()._end;



	using mw::test::parsers::equality;
	using mw::test::parsers::comparison;
	using mw::test::parsers::predicate;
	using mw::test::parsers::skipper;

	using namespace mw::test::data;

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


	mw::test::data::equality e;

	s = "boost::variant<int, double> == 42";
	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	std::cout << "lhs.content: '" << e.lhs.content << "'" << std::endl;
	std::cout << "rhs.content: '" << e.rhs.content << "'" << std::endl;
	BOOST_CHECK(e.lhs.content == "boost::variant<int, double> ");
	BOOST_CHECK(e.rhs.content == "42");
	BOOST_CHECK(!e.tolerance);
	BOOST_CHECK(e.type == relativity_t::absolute);

	s = "x != 3.44 +/- 10%";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(e.inverted);
	BOOST_CHECK(e.lhs.content == "x ");
	BOOST_CHECK(e.rhs.content == "3.44 ");
	BOOST_REQUIRE(e.tolerance);
	BOOST_CHECK(e.tolerance->content == "10");
	BOOST_CHECK(e.type == relativity_t::percent);

	s = "decltype(dings)::max == bla<23>::value +/- tolerance~";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	BOOST_CHECK(e.lhs.content == "decltype(dings)::max ");
	BOOST_CHECK(e.rhs.content == "bla<23>::value ");
	BOOST_REQUIRE(e.tolerance);
	BOOST_CHECK(e.tolerance->content == "tolerance");
	BOOST_CHECK(e.type == relativity_t::relative);

	s = "x == y % z";

	BOOST_CHECK(p(equality, e));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(!e.inverted);
	BOOST_CHECK(e.lhs.content == "x ");
	BOOST_CHECK(e.rhs.content == "y % z");
	BOOST_CHECK(!e.tolerance);



	mw::test::data::comparison c;


	s = "boost::variant<int, double>(23) >= 12";

	BOOST_CHECK(p(comparison, c));

	BOOST_CHECK(c.oper == compare_op::greater_equal);
	BOOST_CHECK(c.lhs.content == "boost::variant<int, double>(23) ");
	BOOST_CHECK(c.rhs.content == "12");



	mw::test::data::predicate pr;

	s = "(x<123>,y ) ";
	BOOST_CHECK(p(mw::test::parsers::arg_list, pr.arg_list));
	BOOST_REQUIRE(pr.arg_list.content.size() == 2);

	BOOST_CHECK(pr.arg_list.content[0] == "x<123>");
	BOOST_CHECK(pr.arg_list.content[1] == "y");


	s = "std::less<int>()(x,y )";

	BOOST_CHECK(p(predicate, pr));

	BOOST_CHECK(pr.name.content == "std::less<int>()");
	BOOST_REQUIRE(pr.arg_list.content.size() == 2);

	BOOST_CHECK(pr.arg_list.content[0] == "x");
	BOOST_CHECK(pr.arg_list.content[1] == "y");


	return 0;
}


