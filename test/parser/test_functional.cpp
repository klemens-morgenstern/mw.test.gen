/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_functional
#include <boost/test/minimal.hpp>

#include <mw/test/parsers/functional.hpp>

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

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parsers;

	namespace data = mw::test::data;
	data::call_trace_decl ctd;


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
	BOOST_CHECK( ctd.name.content == "tested_function(int) ");
	BOOST_CHECK(!ctd.count);


	BOOST_REQUIRE(ctd.content);

	auto &vec = *ctd.content;
	BOOST_REQUIRE(vec.size() == 3);

	BOOST_CHECK(vec[0].content);
	BOOST_CHECK(vec[0].name.content == "&thing<42>::not_overloaded_function ");

	{
		auto v1 = *vec[0].content;
		BOOST_CHECK(v1.size() == 2);
		BOOST_CHECK(v1[0].name.content == "&func1");
		BOOST_CHECK(v1[1].name.content == "&func2\n    ");

	}
	BOOST_REQUIRE(vec[0].count);
	BOOST_CHECK  (*vec[0].count == 2);

	BOOST_CHECK(vec[1].name.content == "&overloaded_func(int)");
	BOOST_CHECK(!vec[1].count);
	BOOST_CHECK(!vec[1].content);

	BOOST_CHECK(vec[2].name.content == "&overloaded_func(double) ");
	BOOST_CHECK(!vec[2].count);
	BOOST_REQUIRE(vec[2].content);
	BOOST_CHECK(vec[2].content->size() == 0);



	/******* test the stub rule ************************************/

	data::stub sb;

	s = "stub thingy<42>::func(int, double) => y(x,z);";

	BOOST_CHECK(p(stub, sb));

	BOOST_CHECK(sb.name.content == "thingy<42>::func(int, double) ");
	BOOST_REQUIRE(sb.return_name);
	BOOST_CHECK(* sb.return_name == "y");

	BOOST_REQUIRE(sb.arg_names.size() == 2);

	BOOST_CHECK(sb.arg_names[0] == "x");
	BOOST_CHECK(sb.arg_names[1] == "z");


	s = "stub func() => void();";

	sb = data::stub();

	BOOST_CHECK(p(stub, sb));

	BOOST_CHECK(sb.name.content == "func() ");
	BOOST_CHECK(!sb.return_name);

	BOOST_CHECK(sb.arg_names.size() == 0);



	/************* test the fake rule *****************************/

	data::fake fk;

	s = "fake ding(int x) => {return x+42;};";

	BOOST_CHECK(p(fake, fk));

	BOOST_CHECK(fk.name.content == "ding(int x) ");
	BOOST_CHECK(fk.func.content == "return x+42;");

	data::functional fc;

	BOOST_CHECK(p(functional, fc));
	BOOST_CHECK(fc.type() == boost::typeindex::type_id<data::fake>());

	return 0;
}
