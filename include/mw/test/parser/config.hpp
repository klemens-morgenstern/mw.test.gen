/**
 * @file   setup.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_CONFIG_HPP_
#define MW_TEST_PARSER_CONFIG_HPP_

#include <boost/spirit/home/x3.hpp>

namespace mw
{
namespace test
{
namespace parser
{

namespace x3 = boost::spirit::x3;
using x3::string;
using x3::char_;
using x3::uint_;
using x3::eol;
using x3::eps;
using x3::no_skip;

}
}
}



#endif /* MW_TEST_PARSER_CONFIG_HPP_ */
