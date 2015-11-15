/**
 * @file   utility.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_UTILITY_HPP_
#define MW_TEST_PARSER_UTILITY_HPP_


#include <mw/test/parser/config.hpp>

namespace mw
{
namespace test
{
namespace parser
{


x3::rule<class log> const id("log");

auto const log_def =
		(!char_(";") >> ";") >> ";";

BOOST_SPIRIT_DEFINE(log);

}
}
}



#endif /* MW_TEST_PARSER_UTILITY_HPP_ */
