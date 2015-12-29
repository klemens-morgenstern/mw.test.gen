/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_group
#include <boost/test/minimal.hpp>

#include <mw/test/parser/group.hpp>

int test_main (int, char**)
{
	std::string s;

	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;


	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	namespace data = mw::test::ast;
	data::group res;

	iterator beg { s.begin()};
	iterator itr { s.begin()};
	iterator end {s.end()  };
	auto p = [&]()
		{
		 	 beg = iterator{ s.begin()};
		 	 itr = iterator{ s.begin()};
			 end = iterator{s.end()  };


			return x3::phrase_parse(itr, end, group, skipper, res);
		};

	s = "test group main = {test1, /*zeugs*/test2<x,y,z,42>};";

	BOOST_CHECK(p());
	BOOST_CHECK(itr == end);


	return 0;
}
