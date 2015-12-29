/**
 * @file   test_file.cpp
 * @date   16.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <vector>

#define BOOST_TEST_MODULE test_file
#include <boost/test/minimal.hpp>

#include <mw/test/parser/file.hpp>
#include <mw/test/parser/comment.hpp>

namespace x3 = boost::spirit::x3;


int test_main (int, char**)
{
	std::string s;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.begin();

	bool res;
	using mw::test::parser::file;
	using mw::test::parser::filename;
	using mw::test::parser::tests_file;
	using mw::test::parser::include;
	using mw::test::parser::skipper;

	namespace d = mw::test::ast;

	auto p = [&](auto rule, auto &result)
		{
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			res = x3::phrase_parse(itr, end, rule, skipper, result);
		};

	s = "adings$-_z.hpp";
	std::string fn;

	p(filename, fn);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(fn == "adings$-_z.hpp");


	s = "_ ";
	fn.clear();
	p(filename, fn);



	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(fn == "_");

	s = "_: ";
	fn.clear();

	p(filename, fn);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == (beg +1 ));
	BOOST_CHECK(fn == "_");

	s = "thingy.hpp";
	fn.clear();

	p(filename, fn);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(fn == "thingy.hpp");

	d::file f;
	s = "file thingy.hpp ;";
	p(file, f);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(f.filename == "thingy.hpp");



	d::include i;
	s = "include other_thingy.h;";
	p(include, i);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(i.filename == "other_thingy.h");


	d::tests_file t;
	s = "/* some comment */\n"
		"tests file dings.h;";
	p(tests_file, t);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(t.filename == "dings.h");


	s = "testsfile dings.h;";
	p(tests_file, t);

	BOOST_CHECK(!res);
	BOOST_CHECK(itr == beg);


	//test the pre commentary
	s = "///Some doc\n  file f;";

	p(file, f);



	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(f.filename == "f");
	BOOST_CHECK(f.doc.head == "Some doc");

	s = "include d.h; //!< post doc\n";

	i.doc.head.clear();
	p(include, i);


	std::cerr << "Head: " << i.doc.head << std::endl;
	std::cerr << "Body: " << i.doc.body << std::endl;

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(i.filename == "d.h");
	BOOST_CHECK(i.doc.head == " post doc");


	s = "/*! pre doc */ tests file i.ipp; //!< post doc\n";

	p(tests_file, t);
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(t.filename == "i.ipp");
	BOOST_CHECK(t.doc.head == " pre doc  post doc");


	return 0;
}
