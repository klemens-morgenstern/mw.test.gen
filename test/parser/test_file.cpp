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

#include <mw/test/parsers/use_file.hpp>
#include <mw/test/parsers/comment.hpp>

#include <type_traits>

namespace x3 = boost::spirit::x3;

#ifndef BOOST_CHECK
#define BOOST_CHECK(Stuff)
#endif

int test_main (int, char**)
{
	std::string s;

	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;


	iterator beg{s.begin()};
	iterator itr{s.begin()};
	iterator end{s.begin()};

	bool res;
	using mw::test::parsers::use_file;
	using mw::test::parsers::use_file_doc;
	using mw::test::parsers::filename;
	using mw::test::parsers::skipper;

	namespace d = mw::test::ast;

	d::use_file uf;

	auto p = [&](auto rule, auto & result)
		{
			using t = std::remove_reference_t<decltype(result)>;
			result = t();
			res = false;
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end()  );
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
	//BOOST_CHECK(itr == (beg +1 ));
	BOOST_CHECK(fn == "_");

	s = "thingy.hpp";
	fn.clear();

	p(filename, fn);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(fn == "thingy.hpp");

	s = "file thingy.hpp ;";
	p(use_file, uf);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.type == d::file);
	BOOST_CHECK(uf.filename == "thingy.hpp");



	s = "include other_thingy.h;";
	p(use_file, uf);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.filename == "other_thingy.h");
	BOOST_CHECK(uf.type == d::include);


	s = "/* some comment */\n"
		"tests file dings.h;";
	p(use_file_doc, uf);

	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.filename == "dings.h");
	BOOST_CHECK(uf.type == d::tests_file);


	s = "testsfile dings.h;";
	p(use_file, uf);

	BOOST_CHECK(!res);
	BOOST_CHECK(itr == beg);


	//test the pre commentary
	s = "///Some doc\n  file f;";

	p(use_file_doc, uf);



	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.filename == "f");
	BOOST_CHECK(uf.doc.head == "Some doc");
	std::cout << "doc: '" << uf.doc.head << "'" << std::endl;
	std::cout << "doc: '" << uf.doc.body << "'" << std::endl;

	BOOST_CHECK(uf.type == d::file);

	s = "include d.h; //!< post doc\n";

	uf.doc.head.clear();
	p(use_file_doc, uf);



	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.filename == "d.h");
	BOOST_CHECK(uf.doc.head == " post doc");
	BOOST_CHECK(uf.type == d::include);

	uf.doc.head.clear();

	s = "/*! pre doc */ tests file i.ipp; //!< post doc\n";

	p(use_file_doc, uf);
	BOOST_CHECK(res);
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uf.filename == "i.ipp");
	BOOST_CHECK(uf.doc.head == " pre doc  post doc");
	BOOST_CHECK(uf.type == d::tests_file);


	return 0;
}
