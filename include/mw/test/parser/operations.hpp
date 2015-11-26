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

#include <iostream>

#include <boost/fusion/include/adapt_struct.hpp>


namespace mw
{
namespace test
{
namespace parser
{

namespace oper
{
auto set_static   = [](auto &ctx){_val(ctx).static_ = true;};
auto set_critical = [](auto &ctx){_val(ctx).set_critical();};
auto set_range 	  = [](auto &ctx){_val(ctx).range();};
auto set_bitwise  = [](auto &ctx){_val(ctx).bitwise = true;};
auto set_data     = [](auto &ctx){_val(ctx).data = _attr(ctx);};
}

auto const assertion_qualification =
		*(lit("static")  [oper::set_static  ]|
		  lit("critical")[oper::set_critical]|
		  lit("ranged")	 [oper::set_range   ]|
		  lit("bitwise") [oper::set_bitwise ]) ;


x3::rule<class func_content> const func_content	("func_content");
x3::rule<class assertion,	data::assertion> 	 const assertion;
x3::rule<class expectation, data::expectation>	 const expectation;
x3::rule<class crit_section> const crit_section	("crit_section");


auto const func_content_def =
		*(
		  assertion    |
		  expectation  |
		  crit_section |
		  log | code_chunk );


auto const assertion_def =
		assertion_qualification >> "assertion" >> code_chunk[oper::set_data] >> ";";



auto const expectation_def =
		assertion_qualification >> "expect" >> code_chunk[oper::set_data] >> ";";



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
