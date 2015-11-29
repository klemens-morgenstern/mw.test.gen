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

#include <mw/test/parser/operations.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

int test_main (int, char**)
{
	std::string s;

	namespace x3 = boost::spirit::x3;
	using namespace mw::test::parser;

	using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;

	namespace data = mw::test::data;

	data::check_entry ce;

	namespace bs = boost::spirit;


	iterator beg {s.begin()};
	iterator itr {s.begin()};
	iterator end {s.end()};

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
	BOOST_CHECK(ec.lvl == data::expectation);
	BOOST_CHECK(itr == end);

	s = "assert execution ;";

	BOOST_CHECK(p(execute_check, ec));
	BOOST_CHECK(ec.critical == false);
	BOOST_CHECK(ec.lvl == data::assertion);
	BOOST_CHECK(itr == end);

	s = " expect no execution ;";
	data::no_execute_check nec;

	BOOST_CHECK(p(no_execute_check, nec));
	BOOST_CHECK(nec.critical == false);
	BOOST_CHECK(nec.lvl == data::expectation);
	BOOST_CHECK(itr == end);

	s = "critical assert no execution ;";

	BOOST_CHECK(p(no_execute_check, nec));
	BOOST_CHECK(nec.critical == true);
	BOOST_CHECK(nec.lvl == data::assertion);
	BOOST_CHECK(itr == end);



	s = "";
	data::check_qualification cq;

	BOOST_CHECK(p(check_qualification, cq));
	BOOST_CHECK(cq.bitwise 	== false);
	BOOST_CHECK(cq.critical == false);
	BOOST_CHECK(cq.static_ 	== false);
	BOOST_CHECK(cq.ranged	== false);
	BOOST_CHECK(itr == end);

	s = " critical static ranged bitwise";

	BOOST_CHECK(p(check_qualification, cq));
	BOOST_CHECK(cq.bitwise 	== true);
	BOOST_CHECK(cq.critical == true);
	BOOST_CHECK(cq.static_ 	== true);
	BOOST_CHECK(cq.ranged	== true);
	BOOST_CHECK(itr == end);


	s = "assert thingy ;";
	data::code_check cc;
	BOOST_CHECK(p(code_check, cc));
	BOOST_CHECK(cc.qual.bitwise  == false);
	BOOST_CHECK(cc.qual.critical == false);
	BOOST_CHECK(cc.qual.static_  == false);
	BOOST_CHECK(cc.qual.ranged 	 == false);
	BOOST_CHECK(cc.lvl == data::level_t::assertion);
	BOOST_CHECK(cc.data.to_string() == "thingy ");

	BOOST_CHECK(itr == end);

	s = "static critical ranged bitwise expect xyz ;";
	BOOST_CHECK(p(code_check, cc));
	BOOST_CHECK(cc.qual.bitwise  == true);
	BOOST_CHECK(cc.qual.critical == true);
	BOOST_CHECK(cc.qual.static_  == true);
	BOOST_CHECK(cc.qual.ranged 	 == true);
	BOOST_CHECK(cc.lvl == data::level_t::expectation);
	BOOST_CHECK(cc.data.to_string() == "xyz ");

	BOOST_CHECK(itr == end);


/*	s = "critical \n"
		"{ assert x ; }";

	data::critical_section cs;

	BOOST_CHECK(p(crit_section, cs));

	BOOST_CHECK(cs.steps.size() == 1);
	BOOST_REQUIRE
			   (cs.steps[0].type() ==
				boost::typeindex::type_id<data::code_check>());

	BOOST_CHECK(boost::get<data::code_check>(
			cs.steps[0]).data.to_string() == "x ;");

*/

	return 0;
}
