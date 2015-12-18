/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>


#include <type_traits>

#include <mw/test/parser/objects.hpp>
#include <mw/test/parser/comment.hpp>

int test_main (int, char**)
{
	std::string s;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;
	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;

	namespace data = mw::test::data;

	data::test_object to;

	iterator beg { s.begin() };
	iterator itr { s.begin() };
	iterator end { s.end()   };
	auto p = [&]()
		{
			to = data::test_object();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			return x3::phrase_parse(itr, end, test_object, skipper, to);
		};

	s = "testclass thingy {};";

	BOOST_CHECK(!p());

	s = "test class thingy {} ;";

	BOOST_CHECK(p());
	BOOST_CHECK(itr == end);
	BOOST_CHECK(to.id.name == "thingy");


	BOOST_CHECK(to.content.size() == 0);
	BOOST_CHECK(to.type == data::class_);


	s = "classification ding : xyz<42>, zyx { execute {} };";


	BOOST_CHECK(p());
	BOOST_CHECK(itr == end);
	BOOST_CHECK(to.id.name == "ding");
	BOOST_CHECK(to.content.size() == 1);
	BOOST_CHECK(to.type == data::classification);


	return 0;
}
