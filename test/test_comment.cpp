/**
 * @file   test_comment.cpp
 * @date   16.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_comment
#include <boost/test/minimal.hpp>
#include <mw/test/parser/comment.hpp>


int test_main (int, char**)
{
	std::string s;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.begin();

	bool res;
	using namespace mw::test::parser;


	std::string parsed;

	auto p = [&](auto rule)
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			res = x3::parse(itr, end, rule, parsed);
		};

	// line comment
	s = "// some comment ä12893\n";

	p(comment);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "// /some comment ä12893\n";

	p(comment);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/// some comment ä12893\n";

	p(comment);

	BOOST_CHECK(!res);
	BOOST_CHECK(itr == beg);


	// block comment
	s = "/*/ some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/* *some comment ä12893\n1*23\n asc#+ */";

	p(comment);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/** some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(!res);
	BOOST_CHECK(itr == beg);

	return 0;
}
