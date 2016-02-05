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

#include <mw/test/parsers/object_content.hpp>
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

    namespace data = mw::test::data;

    auto &beg = parser._include_stack.top()._begin;
    auto itr  = parser._include_stack.top()._begin;
    auto &end = parser._include_stack.top()._end;

    namespace x3 = boost::spirit::x3;
    using namespace mw::test::parsers;

    using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

    namespace data = mw::test::data;

    data::using_decl res;

	auto p = [&](auto rule)
		{
			using t = std::remove_reference_t<decltype(res)>;
			res = t();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};


	s = "using father<42>::execute;";
	BOOST_CHECK(p(using_plain));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(res.action == data::action_t::execute);
	BOOST_CHECK(req_id.name == "father");
	BOOST_REQUIRE(req_id.tpl_args.size() == 1);
	BOOST_CHECK(req_id.tpl_args[0] == "42");

	s = "using dings<x,y>::init for assert throw (std::runtime_error, std::logic_error);";

	BOOST_CHECK(p(using_throw));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(res.action == data::action_t::initialize);

	BOOST_CHECK(req_id.name == "dings");
	BOOST_REQUIRE(req_id.tpl_args.size() == 2);
	BOOST_CHECK(req_id.tpl_args[0] == "x");
	BOOST_CHECK(req_id.tpl_args[1] == "y");

	BOOST_REQUIRE(res.exceptions);
	{
	    auto &cl = *res.exceptions;

	    BOOST_REQUIRE(cl.content.size() == 2);

	    BOOST_CHECK(cl.content[0] == "std::runtime_error");
	    BOOST_CHECK(cl.content[1] == "std::logic_error");
	}

	s = "using xyz::evaluate for assert  any_throw;";

	BOOST_CHECK(p(using_any_throw));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(res.action == data::action_t::evaluate);


	s = "using thingy::eval for expect no_throw;";

	BOOST_CHECK(p(using_no_throw));
	BOOST_CHECK(itr == end);
	BOOST_CHECK(res.action == data::action_t::evaluate);



	s = "using father::operator=;";
	BOOST_CHECK(!p(using_decl));


	return 0;
}
