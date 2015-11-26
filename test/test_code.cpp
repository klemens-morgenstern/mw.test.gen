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


	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	namespace data = mw::test::data;
	std::string res;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.end()  ;
	auto p = [&](auto rule)
		{
			res.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
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

	auto pc = [&]()
		{
			res.clear();
			beg = s.begin();
			itr = s.begin();
			end = s.end();

			iterator ib{itr};
			iterator en{end};

			bool b = x3::parse(ib, en, code_chunk, dc);

			itr = ib.base();

			return b;
		};

	s = "auto l = [this](int i) { return i*i;};";
	BOOST_CHECK(pc());
	BOOST_CHECK(dc.to_string() == "auto l = [this](int i) { return i*i;};");
	BOOST_CHECK(itr == end);

	s = "int i ; xyz";
	BOOST_CHECK(pc());
	BOOST_CHECK(dc.to_string() == "int i ;");
	BOOST_CHECK(itr == end - 4);

	s = "x > 42;";
	BOOST_CHECK(pc());
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.to_string() == "x > 42;");


	return 0;
}
