/**
 * @file   test_utility.cpp
 * @date   19.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <string>

#define BOOST_TEST_MODULE test_utility
#include <boost/test/minimal.hpp>
#include <iostream>
#include <mw/test/parser/utility.hpp>

namespace x3 = boost::spirit::x3;


int test_main (int, char**)
{
	std::string s;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.begin();

	bool res;
	using mw::test::parsers::log;
	using mw::test::parsers::quoted_string;
	using mw::test::parsers::squoted_string;

	std::string parsed;

	auto p = [&](auto rule)
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			res = x3::parse(itr, end, log, parsed);
		};

	s = "log dings;";

	p(log);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(parsed == "dings");

	s = "log some log thingy ';' \"string; \" << other value 223223 ;";
	p(log);

	std::cerr << "'" << parsed << "'" << std::endl;

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(parsed == "some log thingy ';' \"string; \" << other value 223223 ");
	return 0;
}


