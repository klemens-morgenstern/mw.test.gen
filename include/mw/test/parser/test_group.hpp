/**
 * @file   test_runner.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_TEST_GROUP_HPP_
#define MW_TEST_PARSER_TEST_GROUP_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/parser/template.hpp>


namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class test_group> const test_group("test_group");

auto const test_group_def =
		"test" >> string("group") >>
		 -id >> "=" >> "{" >>
		((id >> -tpl_par_list) % ',') >>
		"}";

BOOST_SPIRIT_DEFINE(test_group);

/**
 * template<int, int> test case tpl_test {};
 * test group ti = tpl_test<[232,232,232,232,112],[1,2,3,34]>; //generates a test group
 *
 */


}
}
}
#endif /* MW_TEST_PARSER_TEST_GROUP_HPP_ */
