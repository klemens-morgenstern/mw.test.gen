/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>

#include <mw/test/parsers/actions.hpp>
#include <mw/test/parsers/comment.hpp>

mw::test::data::obj_id req_id;

// STUB the parser.cpp
namespace mw
{
namespace test
{

thread_local parser *_instance = nullptr;


parser::parser()
{
    assert(_instance == nullptr);

    _instance = this;
}


parser &parser::instance()
{
    assert(_instance != nullptr);
    return *_instance;
}


data::object_p parser::get_object(const data::obj_id& in)
{
    req_id = in;
    return nullptr;
}


} /* namespace test */
} /* namespace mw */


int test_main (int, char**)
{
    mw::test::parser parser;

    parser._include_stack.emplace(std::string(""));

    std::string &s = parser._include_stack.top().buffer;


    using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

    namespace x3 = boost::spirit::x3;
    using namespace mw::test::parsers;

    auto &beg = parser._include_stack.top()._begin;
    auto itr  = parser._include_stack.top()._begin;
    auto &end = parser._include_stack.top()._end;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parsers;

	namespace data = mw::test::data;
	data::action_call res;

	auto p = [&](auto rule)
		{
			res = data::action_call();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};

	s = "execute thingy;";

	BOOST_CHECK(p(action_call));
	BOOST_CHECK(itr == end);

	BOOST_CHECK(res.type == data::action_t::execute);
	BOOST_CHECK(res.target.expired());
	BOOST_CHECK(req_id.name == "thingy");


	s = "init/* stuff */ding<42,xyz>//dingsda\n ;";

	BOOST_CHECK(p(action_call));
	BOOST_CHECK(itr == end);

	BOOST_CHECK(res.type == data::action_t::initialize);
    BOOST_CHECK(res.target.expired());

	BOOST_CHECK(req_id.name == "ding");
	BOOST_REQUIRE(req_id.tpl_args.size() == 2);

	BOOST_CHECK(req_id.tpl_args[0] == "42");
	BOOST_CHECK(req_id.tpl_args[1] == "xyz");

	return 0;
}
