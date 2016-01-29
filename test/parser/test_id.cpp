/**
 * @file   test_id.cpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <string>

#define BOOST_TEST_MODULE test_id
#include <boost/test/minimal.hpp>
#include <iostream>
#include <mw/test/parser/id.hpp>

namespace x3 = boost::spirit::x3;


int test_main (int, char**)
{
	std::string s;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.begin();

	bool res;
	using mw::test::parsers::id;

	std::string parsed;

	auto p = [&]
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			res = x3::parse(itr, end, id, parsed);
		};

	s = "_1";
	p();

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(parsed == "_1");

	s = "1";
	p();

	BOOST_CHECK(!res);
	BOOST_CHECK(itr == beg);

	s = "b1";
	p();

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(parsed == "b1");


	s = "dingz#asd";
	p();

	BOOST_CHECK(res);
	BOOST_CHECK(itr == (beg + 5));
	BOOST_CHECK(parsed == "dingz");

	s = "part1 _ part2";
	p();

	BOOST_CHECK(res);
	BOOST_CHECK(itr == (beg + 5));
	BOOST_CHECK(parsed == "part1");

	return 0;
}
