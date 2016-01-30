/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>
#include <boost/test/results_reporter.hpp>

#include <mw/test/parser.hpp>
#include <mw/test/parsers/code.hpp>



int test_main (int, char**)
{
    mw::test::parser parser;

    parser.include_stack.emplace(std::string(""));

	std::string &s = parser.include_stack.top().buffer;


	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parsers;

	namespace data = mw::test::data;
	std::string res;

	auto &beg = parser.include_stack.top()._begin;
	auto itr  = parser.include_stack.top()._begin;
	auto &end = parser.include_stack.top()._end;

	auto p = [&](auto rule)
		{
			res.clear();
			beg = iterator(s.cbegin());
			itr = iterator(s.cbegin());
			end = iterator(s.cend());
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
            beg = iterator(s.cbegin());
            itr = iterator(s.cbegin());
            end = iterator(s.cend());

			bool b = x3::parse(itr, end, rule, dc);

			return b;
		};

	s = "auto l = [this](int i) { return i*i;};";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(dc.content == "auto l = [this](int i) { return i*i;}");
	BOOST_CHECK(itr == end);
	std::cout << "Content: '" << dc.content << "'" << std::endl;

	s = "int i ; xyz";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(dc.content == "int i ");
	BOOST_CHECK(itr.base() == end.base() - 4);

	s = "x > 42;";
	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.content == "x > 42");

	s = "{ asdklklss,d, {} };";
	BOOST_CHECK(pc(code_section));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.content == " asdklklss,d, {} ");

	s= "x == 42 ;";

	BOOST_CHECK(pc(code_chunk));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(dc.content == "x == 42 ");


	data::code_list cl;

	auto pc2 = [&]()
		{
			res.clear();
            beg = iterator(s.cbegin());
            itr = iterator(s.cbegin());
            end = iterator(s.cend());


			bool b = x3::parse(itr, end, code_list, cl);

			return b;
		};

	s = "thingy, boost::variant<int, void*>, dings,xyz";

	BOOST_CHECK(pc2());
	BOOST_CHECK(itr == end);
	BOOST_REQUIRE(cl.content.size() == 4);

	BOOST_CHECK(cl.content[0] == "thingy");
	BOOST_CHECK(cl.content[1] == "boost::variant<int, void*>");
	BOOST_CHECK(cl.content[2] == "dings");
	BOOST_CHECK(cl.content[3] == "xyz");
	//BOOST_CHECK(cl.to_string() == "thingy, boost::variant<int, void*>, dings,xyz");




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
