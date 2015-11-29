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

struct level_t : x3::symbols<data::level_t>
{
	level_t()
	{
		add	("assertion",  data::assertion)
			("expecation", data::expectation)
			;
	}
} level;

namespace oper
{
auto set_static   = [](auto &ctx){_val(ctx).static_ = true;};
auto set_critical = [](auto &ctx){_val(ctx).set_critical();};
auto set_range 	  = [](auto &ctx){_val(ctx).range();};
auto set_bitwise  = [](auto &ctx){_val(ctx).bitwise = true;};
auto set_data     = [](auto &ctx){_val(ctx).data = _attr(ctx);};
auto set_location = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx).location = itr;
		};
}

auto const assertion_qualification =
		*(lit("static")  [oper::set_static  ]|
		  lit("critical")[oper::set_critical]|
		  lit("ranged")	 [oper::set_range   ]|
		  lit("bitwise") [oper::set_bitwise ]) ;


x3::rule<class func_content> 					 			const func_content;
x3::rule<class execute_check, 	 data::execute_check>		const execute_check;
x3::rule<class no_execute_check, data::no_execute_check>	const no_execute_check;
x3::rule<class crit_section> 					 			const crit_section;



auto const execute_check_def =
		code_location >> level >> lit("execution") >> ";";

auto const no_execute_check_def =
		code_location >> level >> "no">> lit("execution") >> ";";



auto const func_content_def =
		*(
//		  assertion    |
//		  expectation  |
		  crit_section |
		  log | code_chunk );

auto const crit_section_def =
		"critical" >> char_('{') >> func_content >> '}';



BOOST_SPIRIT_DEFINE(func_content);
BOOST_SPIRIT_DEFINE(execute_check);
BOOST_SPIRIT_DEFINE(no_execute_check);
BOOST_SPIRIT_DEFINE(crit_section);


}
}
}



#endif /* MW_TEST_PARSER_OPERATIONS_HPP_ */
