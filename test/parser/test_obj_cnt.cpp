/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>


#include <type_traits>

#include <mw/test/parser/object_content.hpp>
#include <mw/test/parser/comment.hpp>

int test_main (int, char**)
{
	std::string s;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;
	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

	namespace data = mw::test::ast;

	iterator beg { s.begin() };
	iterator itr { s.begin() };
	iterator end { s.end()   };
	auto p = [&](auto rule, auto &res)
		{
			using t = std::remove_reference_t<decltype(res)>;
			res = t();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};

	data::using_plain up;

	s = "using father<42>::execute;";
	BOOST_CHECK(p(using_plain, up));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(up.action == data::execute);
	BOOST_CHECK(up.id.name == "father");
	BOOST_REQUIRE(up.id.tpl_args.size() == 1);
	BOOST_CHECK(up.id.tpl_args[0] == "42");


	data::using_throw ut;

	s = "using dings<x,y>::init for assert throw (std::runtime_error, std::logic_error);";

	BOOST_CHECK(p(using_throw, ut));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(ut.action == data::initialize);

	BOOST_CHECK(ut.id.name == "dings");
	BOOST_REQUIRE(ut.id.tpl_args.size() == 2);
	BOOST_CHECK(ut.id.tpl_args[0] == "x");
	BOOST_CHECK(ut.id.tpl_args[1] == "y");

	BOOST_REQUIRE(ut.exceptions.size() == 2);

	BOOST_CHECK(ut.exceptions.data[0] == "std::runtime_error");
	BOOST_CHECK(ut.exceptions.data[1] == "std::logic_error");


	data::using_any_throw uat;

	s = "using xyz::evaluate for assert  any throw;";

	BOOST_CHECK(p(using_any_throw, uat));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(uat.action == data::evaluate);


	data::using_no_throw unt;

	s = "using thingy::eval for expect no throw;";

	BOOST_CHECK(p(using_no_throw, unt));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(unt.action == data::evaluate);




	data::using_decl ud;
	s = "using father::operator=;";
	BOOST_CHECK(!p(using_decl, ud));


	return 0;
}
