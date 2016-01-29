/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>

#include <mw/test/parser/code.hpp>

int test_main (int, char**)
{
	std::string s;


	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parsers;

	namespace data = mw::test::ast;
	std::string res;

	auto beg = s.cbegin();
	auto itr = s.cbegin();
	auto end = s.cend()  ;
	auto p = [&](auto rule)
		{
			res.clear();
			beg = s.cbegin();
			itr = s.cbegin();
			end = s.cend();
			return x3::parse(itr, end, rule, res);
		};

	s = "[ asdkl ]";
	BOOST_CHECK(p(square_par_code_chunk));
	BOOST_CHECK(res == "[ asdkl ]");
	BOOST_CHECK(itr == end);


	s = "( asdkl )";
	BOOST_CHECK(p(round_par_code_chunk));
	BOOST_CHECK(res == "( asdkl )");
	BOOST_CHECK(itr == end);


	s = "{ asdkl }";
	BOOST_CHECK(p(curly_par_code_chunk));
	BOOST_CHECK(res == "{ asdkl }");
	BOOST_CHECK(itr == end);


	s = "< asdkl >";
	BOOST_CHECK(p(pointy_par_code_chunk));
	BOOST_CHECK(res == "< asdkl >");
	BOOST_CHECK(itr == end);



	data::code dc;

	auto pc = [&](auto rule)
		{
			res.clear();
			beg = s.cbegin();
			itr = s.cbegin();
			end = s.cend();

			iterator ib{itr};
			iterator en{end};

			bool b = x3::parse(ib, en, rule, dc);

			itr = ib.base();

			return b;
		};

	s = "auto l = [this](int i) { return i*i;};";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(dc.to_string() == "auto l = [this](int i) { return i*i;}");
	BOOST_CHECK(itr == end);

	s = "int i ; xyz";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(dc.to_string() == "int i ");
	BOOST_CHECK(itr == end - 4);

	s = "x > 42;";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.to_string() == "x > 42");

	s = "{ asdklklss,d, {} };";
	BOOST_CHECK(pc(code_section));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.to_string() == " asdklklss,d, {} ");

	s= "x == 42 ;";

	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.to_string() == "x == 42 ");


	data::code_list cl;

	auto pc2 = [&]()
		{
			res.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();

			iterator ib{itr};
			iterator en{end};

			bool b = x3::parse(ib, en, code_list, cl);

			itr = ib.base();

			return b;
		};

	s = "thingy, boost::variant<int, void*>, dings,xyz";

	BOOST_CHECK(pc2());
	BOOST_CHECK(itr == end);
	BOOST_REQUIRE(cl.size() == 4);

	BOOST_CHECK(cl.data[0] == "thingy");
	BOOST_CHECK(cl.data[1] == "boost::variant<int, void*>");
	BOOST_CHECK(cl.data[2] == "dings");
	BOOST_CHECK(cl.data[3] == "xyz");
	BOOST_CHECK(cl.to_string() == "thingy, boost::variant<int, void*>, dings,xyz");




	s = "invalid function{42};";

	BOOST_CHECK(!pc(code_function));

	s = "valid function(){stuff;}"; //not parsed with phrase_parse...

	BOOST_CHECK(pc(code_function));
	BOOST_CHECK(itr == end);

	s = "another valid function(int i = 42, double x = 0.221){}";

	BOOST_CHECK(pc(code_function));
	BOOST_CHECK(itr == end);



	return 0;
}
