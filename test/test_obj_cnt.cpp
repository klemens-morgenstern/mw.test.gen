/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>

#include <mw/test/parser/object_content.hpp>
#include <mw/test/parser/comment.hpp>

int test_main (int, char**)
{
	std::string s;



	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	namespace data = mw::test::data;
	data::action_call res;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.end()  ;
	auto p = [&](auto rule)
		{
			res = data::action_call();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};



	return 0;
}
