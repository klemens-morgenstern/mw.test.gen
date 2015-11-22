/**
 * @file   operations.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OPERATIONS_HPP_
#define MW_TEST_PARSER_OPERATIONS_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/utility.hpp>
#include <mw/test/data/operations.hpp>


#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
		mw::test::data::assignment,
		(std::string, lhs)
		(std::string, rhs)
);

namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class


//auto const operator = '=' | lit("->") >> '<' | lit('>') | "<=" | lit(">=") | "==" | lit("!=");

x3::rule<class assertion_qualification> const assertion_qualification("assertion_qualification");

auto const assertion_qualification_def =
		*(string("static") 	 |
		  string("critical") |
		  string("ranged")	 |
		  string("bitwise")) ;


x3::rule<class func_content> const func_content("func_content");
x3::rule<class assertion> 	 const assertion("assertion");
x3::rule<class expectation>	 const expectation("expectation");
x3::rule<class crit_section> const crit_section("crit_section");


auto const func_content_def =
		*(
		  assertion    |
		  expectation  |
		  crit_section |
		  log | code );


auto const assertion_def =
		assertion_qualification >> "assert" >> code >> ";";



auto const expectation_def =
		assertion_qualification >> "expect" >> code >> ";";



auto const crit_section_def =
		"critical" >> char_('{') >> func_content >> '}';




BOOST_SPIRIT_DEFINE(func_content);
BOOST_SPIRIT_DEFINE(assertion);
BOOST_SPIRIT_DEFINE(expectation);
BOOST_SPIRIT_DEFINE(crit_section);


}
}
}



#endif /* MW_TEST_PARSER_OPERATIONS_HPP_ */
