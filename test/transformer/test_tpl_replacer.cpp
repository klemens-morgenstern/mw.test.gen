/**
 * @file   test_tpl_replacer.cpp
 * @date   20.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_tpl_replacer
#include <boost/test/minimal.hpp>

#include <mw/test/template_instantiate.hpp>

int test_main (int, char**)
{
	using t = mw::test::template_instantiate;
	using r = t::replacement;
	using namespace std;

	vector<r> reps = {r{"Thingy", "OtherThingy"}, r{"x", "y"}};

	std::string test_string =
			"class Thingy : Thingy2\n"
			"{ \n"
			"	int x;\n"
			"   //the value is set to x.\n"
			"	string s = 'x';\n"
			"	/*** Thingy is x ***/\n"
			"	string x = \"Thingy\";\n"
			"};";


	auto res = t::replace(test_string, reps);


	std::string expected =
			"class OtherThingy : Thingy2\n"
						"{ \n"
						"	int y;\n"
						"   //the value is set to x.\n"
						"	string s = 'x';\n"
						"	/*** Thingy is x ***/\n"
						"	string y = \"Thingy\";\n"
						"};";

	BOOST_CHECK(res == expected);


	return 0;
}
