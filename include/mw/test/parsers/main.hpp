/**
 * @file   id.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_MAIN_HPP_
#define MW_TEST_PARSER_MAIN_HPP_


#include <mw/test/parsers/objects.hpp>
#include <mw/test/parsers/group.hpp>
#include <mw/test/parsers/use_file.hpp>

#include <string>

namespace mw
{
namespace test
{
namespace parsers
{


x3::rule<class main> const main;

auto const main_def =
		*omit[
			  	test_object // no need to do anything, since objects are already constructed in the tule
			  |	group      [l([](auto &c){parser::instance().add_group(std::move(x3::_attr(c)));})]
			  | use_file   [l([](auto &c){parser::instance().add_use_file(std::move(x3::_attr(c)));})]
			  ];


BOOST_SPIRIT_DEFINE(main);

}
}
}


#endif /* MW_TEST_PARSER_ID_HPP_ */
