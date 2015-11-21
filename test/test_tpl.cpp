/**
 * @file   test_tpl.cpp
 * @date   21.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_tpl
#include <boost/test/minimal.hpp>
#include <mw/test/parser/template.hpp>
#include <type_traits>

int test_main (int, char**)
{
	std::string input;
	std::string result;

	auto beg = input.begin();
	auto itr = beg;
	auto end = beg;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;
	namespace d = mw::test::data;

	auto l = [&](auto rule, auto & prod)
	{
		prod = std::remove_reference_t<decltype(prod)>();
		beg = input.begin();
		itr = input.begin();
		end = input.end();

		return x3::parse(itr, end, rule, prod);
	};

	input = "[ adlkjasdzi8998 ..; ] ";

	BOOST_CHECK(l(tpl_square_par, result));
	BOOST_CHECK(result == "[ adlkjasdzi8998 ..; ]");
	BOOST_CHECK(itr == (end-1));


	input = "[ {} ]";

	BOOST_CHECK(l(tpl_square_par, result));
	BOOST_CHECK(result == "[ {} ]");
	BOOST_CHECK(itr == end);


	input = "{ asdlköä... ; }";

	BOOST_CHECK(l(tpl_curly_par, result));
	BOOST_CHECK(result == "{ asdlköä... ; }");
	BOOST_CHECK(itr == end);

	input = "( -.ä;ö )";

	BOOST_CHECK(l(tpl_round_par, result));
	BOOST_CHECK(result == "( -.ä;ö )");
	BOOST_CHECK(itr == end);


	input = "< -.äö ; >";

	BOOST_CHECK(l(tpl_pointy_par, result));
	BOOST_CHECK(result == "< -.äö ; >");
	BOOST_CHECK(itr == end);

	input = "x";

	BOOST_CHECK(l(tpl_par_step, result));
	BOOST_CHECK(result == "x");
	BOOST_CHECK(itr == end);

	input = "<,x>";

	BOOST_CHECK(l(tpl_par_step, result));
	BOOST_CHECK(result == "<,x>");
	BOOST_CHECK(itr == end);

	input = "some <asd,{}> word {123öö} axx [ <> ] ((((,)))) xy";

	BOOST_CHECK(l(tpl_par, result));
	BOOST_CHECK(result == "some <asd,{}> word {123öö} axx [ <> ] ((((,)))) xy");
	BOOST_CHECK(itr == end);

	input = "some, <asd,{}> word {123öö} axx [ <> ] ((((,)))) xy";

	BOOST_CHECK(l(tpl_par, result));
	BOOST_CHECK(result == "some");
	BOOST_CHECK(itr == beg+4);


	input = "thingy";

	d::tpl_arg ta;

	BOOST_CHECK(l(tpl_arg, ta));
	BOOST_CHECK(ta.name == "thingy");
	BOOST_CHECK(ta.def_arg.empty());
	BOOST_CHECK(itr == end);

	input = "i=42";

	BOOST_CHECK(l(tpl_arg, ta));
	BOOST_CHECK(ta.name == "i");
	BOOST_CHECK(ta.def_arg == "42");
	BOOST_CHECK(itr == end);

	input = "<stuff,x=32,asd=int()>";

	std::vector<d::tpl_arg> rv;

	BOOST_CHECK(l(tpl_decl, rv));
	BOOST_CHECK(itr == end);
	BOOST_REQUIRE(rv.size() == 3);
	BOOST_CHECK(rv[0].name == "stuff");
	BOOST_CHECK(rv[0].def_arg.empty());
	BOOST_CHECK(rv[1].name	  == "x");
	BOOST_CHECK(rv[1].def_arg == "32");
	BOOST_CHECK(rv[2].name	  == "asd");
	BOOST_CHECK(rv[2].def_arg == "int()");


	input = "<stuff,32,boost::variant<int>,decltype(23.44 * 2),{}>";

	std::vector<std::string> rvs;

	BOOST_CHECK(l(tpl_par_list, rvs));
	BOOST_CHECK(itr == end);
	BOOST_REQUIRE(rvs.size() == 5);
	BOOST_CHECK(rvs[0] == "stuff");
	BOOST_CHECK(rvs[1] == "32");
	BOOST_CHECK(rvs[2] == "boost::variant<int>");
	BOOST_CHECK(rvs[3] == "decltype(23.44 * 2)");
	BOOST_CHECK(rvs[4] == "{}");

	return 0;
}
