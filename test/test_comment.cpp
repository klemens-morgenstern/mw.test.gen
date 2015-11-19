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

	BOOST_CHECK(parsed.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "// /some comment ä12893\n";

	p(comment);

	BOOST_CHECK(parsed.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);


	// block comment
	s = "/*/ some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(parsed.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/* *some comment ä12893\n1*23\n asc#+ */";

	p(comment);

	BOOST_CHECK(parsed.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	//ok, so comments work. now , does it work when implemented via the skipper

	s = "//stuff \n x /** documentation */ z";

	parsed.clear();
	beg = s.begin();
	itr = s.begin();
	end = s.end();


	res = x3::phrase_parse(itr, end, char_('x') >> char_('z') , skipper, parsed);

	BOOST_CHECK(parsed == "xz");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);


	/************ Now test documentation rules *****************/

	mw::test::data::doc d;

	auto p2 = [&](auto rule)
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();

			d.body.clear();
			d.head.clear();

			res = x3::parse(itr, end, rule, d);
		};


	s = "/// some comment ä12893\n";

	p2(comment_pre_doc);

	BOOST_CHECK(parsed.empty());

	BOOST_CHECK(d.head == " some comment ä12893");
	BOOST_CHECK(d.body.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);


	s = "///< some comment ä12893\n";

	p2(comment_post_doc);

	BOOST_CHECK(parsed.empty());

	BOOST_CHECK(d.head == " some comment ä12893");
	BOOST_CHECK(d.body.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/** some comment ä12893\n */";

	p2(comment_pre_doc);

	std::cerr << d.head << std::endl;

	BOOST_CHECK(d.head == " some comment ä12893\n ");
	BOOST_CHECK(d.body.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/**< some comment ä12893\n */";

	p2(comment_post_doc);

	BOOST_CHECK(d.head == " some comment ä12893\n ");
	BOOST_CHECK(d.body.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	/*** look if the pre thingy fails */

	p2(comment_pre_doc);
	BOOST_CHECK(!res);

	//ok, works, now see if combinations work as they should

	s = "///head\n/**body*//** body2*/";

	p2(comment_pre_doc);

	BOOST_CHECK(d.head == "head");
	BOOST_CHECK(d.body == "body body2");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);


	return 0;
}
