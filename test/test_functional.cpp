/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_functional
#include <boost/test/minimal.hpp>

#include <mw/test/parser/functional.hpp>

int test_main (int, char**)
{
	std::string s;


	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	namespace data = mw::test::data;
	data::call_trace_decl ctd;

	iterator beg {s.begin()};
	iterator itr {s.begin()};
	iterator end {s.end()  };
	auto p = [&](auto rule, auto & data)
		{
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end()  );
			return x3::phrase_parse(itr, end, rule, skipper, data);
		};


	s = "call trace x for tested_function(int) =>\n"
		"{\n"
		"    &thing<42>::not_overloaded_function =>\n"
		"    {\n"
		"        &func1,\n"
		"        &func2\n"
		"    } * 2, //expectation\n"
		"    &overloaded_func(int),\n"
		"    &overloaded_func(double) => {}\n"
		"};";

	BOOST_CHECK(p(call_trace_decl, ctd));

	BOOST_CHECK( ctd.id == "x");
	BOOST_CHECK( ctd.name.to_string() == "tested_function(int) ");
	BOOST_CHECK(!ctd.count);


	BOOST_REQUIRE(ctd.content);

	auto &vec = *ctd.content;
	BOOST_REQUIRE(vec.size() == 3);

	BOOST_CHECK(vec[0].content);
	BOOST_CHECK(vec[0].name.to_string() == "&thing<42>::not_overloaded_function ");

	{
		auto v1 = *vec[0].content;
		BOOST_CHECK(v1.size() == 2);
		BOOST_CHECK(v1[0].name.to_string() == "&func1");
		BOOST_CHECK(v1[1].name.to_string() == "&func2\n    ");

	}
	BOOST_REQUIRE(vec[0].count);
	BOOST_CHECK  (*vec[0].count == 2);

	BOOST_CHECK(vec[1].name.to_string() == "&overloaded_func(int)");
	BOOST_CHECK(!vec[1].count);
	BOOST_CHECK(!vec[1].content);

	BOOST_CHECK(vec[2].name.to_string() == "&overloaded_func(double) ");
	BOOST_CHECK(!vec[2].count);
	BOOST_REQUIRE(vec[2].content);
	BOOST_CHECK(vec[2].content->size() == 0);



	/******* test the stub rule ************************************/

	data::stub sb;

	s = "stub thingy<42>::func(int, double) => y(x,z);";

	BOOST_CHECK(p(stub, sb));

	BOOST_CHECK(sb.name.to_string() == "thingy<42>::func(int, double) ");
	BOOST_REQUIRE(sb.return_name);
	BOOST_CHECK(* sb.return_name == "y");

	BOOST_REQUIRE(sb.arg_names.size() == 2);

	BOOST_CHECK(sb.arg_names[0] == "x");
	BOOST_CHECK(sb.arg_names[1] == "z");


	s = "stub func() => void();";

	sb = data::stub();

	BOOST_CHECK(p(stub, sb));

	BOOST_CHECK(sb.name.to_string() == "func() ");
	BOOST_CHECK(!sb.return_name);

	BOOST_CHECK(sb.arg_names.size() == 0);



	/************* test the fake rule *****************************/

	data::fake fk;

	s = "fake ding(int x) => {return x+42;};";

	BOOST_CHECK(p(fake, fk));

	BOOST_CHECK(fk.name.to_string() == "ding(int x) ");
	BOOST_CHECK(fk.func.to_string() == "return x+42;");

	data::functional fc;

	BOOST_CHECK(p(functional, fc));
	BOOST_CHECK(fc.type() == boost::typeindex::type_id<data::fake>());

	return 0;
}
