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

	using entity = mw::test::data::entity;
	entity pre; //means a pre-doc
	entity post;//menas a post-doc

	std::string parsed;

	auto p = [&](auto rule)
		{
			parsed.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			pre. doc.clear();
			post.doc.clear();
			entity::set_entity(post);
			res = x3::parse(itr, end, rule, parsed);
			entity::set_entity(pre);
		};

	// line comment
	s = "// some comment ä12893\n";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(post.doc.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "// /some comment ä12893\n";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(post.doc.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/// some comment ä12893\n";

	p(comment);

	BOOST_CHECK(!pre.doc.empty());
	BOOST_CHECK(post.doc.empty());

	BOOST_CHECK( pre.doc == " some comment ä12893");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);


	s = "///< some comment ä12893\n";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(!post.doc.empty());

	BOOST_CHECK(post.doc == " some comment ä12893");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);



	// block comment
	s = "/*/ some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(post.doc.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/* *some comment ä12893\n1*23\n asc#+ */";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(post.doc.empty());
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/** some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(!pre.doc.empty());
	BOOST_CHECK(post.doc.empty());
	BOOST_CHECK(pre.doc == " some comment ä12893\n ");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	s = "/**< some comment ä12893\n */";

	p(comment);

	BOOST_CHECK(pre. doc.empty());
	BOOST_CHECK(!post.doc.empty());
	BOOST_CHECK(post.doc == " some comment ä12893\n ");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	//ok, so comments work. now , does it work when implemented via the skipper

	s = "//stuff \n x /** documentation */ z";

	parsed.clear();
	beg = s.begin();
	itr = s.begin();
	end = s.end();
	pre. doc.clear();
	post.doc.clear();
	entity::set_entity(post);

	res = x3::phrase_parse(itr, end, char_('x') >> char_('z') , skipper, parsed);

	entity::set_entity(pre);

	BOOST_CHECK(!pre.doc.empty());
	BOOST_CHECK(post.doc.empty());

	BOOST_CHECK(parsed == "xz");
	BOOST_CHECK(pre.doc == " documentation ");
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);

	return 0;
}
