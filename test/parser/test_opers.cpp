/**
 * @file   test_oper.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <boost/core/demangle.hpp>
#include <type_traits>

#define BOOST_TEST_MODULE test_oper
#include <boost/test/minimal.hpp>

#include <mw/test/parsers/operations.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>


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
    std::string res;

    auto &beg = parser._include_stack.top()._begin;
    auto itr  = parser._include_stack.top()._begin;
    auto &end = parser._include_stack.top()._end;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parsers;

	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

	namespace data = mw::test::data;

	data::check_entry ce;

	namespace bs = boost::spirit;


	auto p = [&](auto rule, auto & res)
		{
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			using t = std::remove_reference_t<decltype(res)>;
			res = t();
			return x3::phrase_parse(itr, end, rule, skipper, res);
		};

	s = "critical expect execution ;";
	data::execute_check ec;

	BOOST_CHECK(p(execute_check, ec));
	BOOST_CHECK(ec.critical == true);
	BOOST_CHECK(ec.lvl == data::level_t::expectation);
	BOOST_CHECK(itr == end);

	s = "assert execution ;";

	BOOST_CHECK(p(execute_check, ec));
	BOOST_CHECK(ec.critical == false);
	BOOST_CHECK(ec.lvl == data::level_t::assertion);
	BOOST_CHECK(itr == end);

	s = " expect no execution ;";
	data::no_execute_check nec;

	BOOST_CHECK(p(no_execute_check, nec));
	BOOST_CHECK(nec.critical == false);
	BOOST_CHECK(nec.lvl == data::level_t::expectation);
	BOOST_CHECK(itr == end);

	s = "critical assert no execution ;";

	BOOST_CHECK(p(no_execute_check, nec));
	BOOST_CHECK(nec.critical == true);
	BOOST_CHECK(nec.lvl == data::level_t::assertion);
	BOOST_CHECK(itr == end);


	using boost::typeindex::type_id;

	s = "assert thingy ;";
	data::code_check cc;
	BOOST_CHECK(p(code_check, cc));
	BOOST_CHECK(cc.qualification.bitwise  == false);
	BOOST_CHECK(cc.qualification.critical == false);
	BOOST_CHECK(cc.qualification.static_  == false);
	BOOST_CHECK(cc.qualification.ranged 	 == false);
	BOOST_CHECK(cc.lvl == data::level_t::assertion);
	BOOST_REQUIRE(cc.data.type() == type_id<data::code>());
	{
	    auto & c = boost::get<data::code>(cc.data);
	    BOOST_CHECK(c.content == "thingy ");
	}
	BOOST_CHECK(itr == end);

	s = "static assert x == 42 ;";
	BOOST_CHECK(p(code_check, cc));
	BOOST_CHECK(cc.qualification.bitwise  == false);
	BOOST_CHECK(cc.qualification.critical == false);
	BOOST_CHECK(cc.qualification.static_  == true);
	BOOST_CHECK(cc.qualification.ranged 	 == false);
	BOOST_CHECK(cc.lvl == data::level_t::assertion);
    BOOST_REQUIRE(cc.data.type() == type_id<data::equality>());
    {
        auto & e = boost::get<data::equality>(cc.data);
        BOOST_CHECK(e.lhs.content == "x ");
        BOOST_CHECK(e.rhs.content == "42 ");

    }

	BOOST_CHECK(itr == end);



	s = "static critical ranged bitwise expect xyz ;";
	BOOST_CHECK(p(code_check, cc));
	BOOST_CHECK(cc.qualification.bitwise  == true);
	BOOST_CHECK(cc.qualification.critical == true);
	BOOST_CHECK(cc.qualification.static_  == true);
	BOOST_CHECK(cc.qualification.ranged 	 == true);
	BOOST_CHECK(cc.lvl == data::level_t::expectation);
    BOOST_REQUIRE(cc.data.type() == type_id<data::code>());
    {
       // auto & c = boost::get<data::code>(cc.data);
        //BOOST_CHECK(c.content == "xyz ");
    }
	BOOST_CHECK(itr == end);


	s = "critical \n"
		"{ assert x ; }";

	data::critical_section cs;

	BOOST_CHECK(p(critical_section, cs));

	BOOST_REQUIRE(cs.steps.size() == 1);
	BOOST_REQUIRE
			   (cs.steps[0].type() ==
				boost::typeindex::type_id<data::code_check>());
	{
	    auto cc = boost::get<data::code_check>(cs.steps[0]);
	    BOOST_REQUIRE(cc.data.type() == type_id<data::code>());
        auto & c = boost::get<data::code>(cc.data);
        BOOST_CHECK(c.content == "x ");
	}

	s = " assert throw (std::exception, std::runtime_error)\n"
		"{ expect x == 42 ;\n"
		"};";

	data::throw_check tc;

	BOOST_CHECK(p(throw_check, tc));

	BOOST_REQUIRE(tc.steps.size() == 1);
	BOOST_REQUIRE(tc.exceptions.content.size() == 2);

	BOOST_CHECK(itr == end);

	BOOST_REQUIRE
			   (tc.steps[0].type() ==
				boost::typeindex::type_id<data::code_check>());
	{
	    auto & cc = boost::get<data::code_check>(tc.steps[0]);

	    BOOST_REQUIRE(cc.data.type() == type_id<data::equality>());

        auto & e = boost::get<data::equality>(cc.data);
        BOOST_CHECK(e.lhs.content == "x ");
        BOOST_CHECK(e.rhs.content == "42 ");


	}
	BOOST_CHECK(tc.exceptions.content[0] == "std::exception");
	BOOST_CHECK(tc.exceptions.content[1] == "std::runtime_error");


	s = "critical expect no_throw { };";

	data::no_throw_check ntc;

	BOOST_CHECK(p(no_throw_check, ntc));

	BOOST_CHECK(itr == end);
	BOOST_CHECK(ntc.critical);
	BOOST_CHECK(ntc.lvl == data::level_t::expectation);
	BOOST_CHECK(ntc.steps.size() == 0);


	s = " assert any_throw { int i = 42; };";

	data::any_throw_check atc;

	BOOST_CHECK(p(any_throw_check, atc));

	BOOST_CHECK(itr == end);
	BOOST_CHECK(!atc.critical);
	BOOST_CHECK(atc.lvl == data::level_t::assertion);
	BOOST_REQUIRE(atc.steps.size() == 1);



	s = "///some comment\nassert no execution; ///< 2\n/**<postfix comment*/";

	BOOST_CHECK(p(check_entry, ce));

	BOOST_CHECK(itr == end);
	BOOST_REQUIRE(ce.type() == boost::typeindex::type_id<data::no_execute_check>());

	{
		auto & v = boost::get<data::no_execute_check>(ce);

		BOOST_CHECK(v.critical == false);
		BOOST_CHECK(v.lvl == data::level_t::assertion);
		BOOST_CHECK(v.doc.head == "some comment 2");
		BOOST_CHECK(v.doc.body == "postfix comment");

	}
	return 0;
}
