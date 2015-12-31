/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>

#include <mw/test/parser/actions.hpp>
#include <mw/test/parser/comment.hpp>

int test_main (int, char**)
{
	std::string s;



	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	namespace data = mw::test::ast;
	data::action_call res;

	auto beg = s.begin();
	auto itr = s.begin();
	auto end = s.end()  ;
	auto p = [&](auto rule)
		{
			res = data::action_call();
			beg = s.begin();
			itr = s.begin();
			end = s.end();
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};

	s = "execute thingy;";

	BOOST_CHECK(p(action_call));
	BOOST_CHECK(itr == end);

	BOOST_CHECK(res.action == data::execute);
	BOOST_CHECK(res.id == "thingy");
	BOOST_CHECK(res.tpl_par.size() == 0);


	s = "init/* stuff */ding<42,xyz>//dingsda\n ;";

	BOOST_CHECK(p(action_call));
	BOOST_CHECK(itr == end);

	BOOST_CHECK(res.action == data::initialize);
	BOOST_CHECK(res.id == "ding");
	BOOST_REQUIRE(res.tpl_par.size() == 2);

	BOOST_CHECK(res.tpl_par[0] == "42");
	BOOST_CHECK(res.tpl_par[1] == "xyz");

	return 0;
}
