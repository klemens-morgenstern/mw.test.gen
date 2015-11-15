/**
 * @file   id.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_ID_HPP_
#define MW_TEST_PARSER_ID_HPP_

#include <mw/test/parser/config.hpp>


namespace mw
{
namespace test
{
namespace parser
{


x3::rule<class id> const id("id");

auto const id_def =
		(char_("_") | char_("A", "Z") | char_("a", "z")) >>
		*(char_("_") | char_("A", "Z") | char_("a", "z") | char_("0", "9"));


BOOST_SPIRIT_DEFINE(id);

}
}
}


#endif /* MW_TEST_PARSER_ID_HPP_ */
