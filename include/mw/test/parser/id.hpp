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
#include <string>

namespace mw
{
namespace test
{
namespace parser
{


x3::rule<class id, std::string> const id("id");

auto const id_def =
		no_skip[
			(
				char_('_') |
				char_('A', 'Z') |
				char_('a', 'z')
				//workaround
				| char_("AZaz")
		 	 ) >>
			*(
				char_('_') |
				char_('A', 'Z') |
				char_('a', 'z') |
				char_('0', '9')
				//workaround for current x3 spirit version
				| char_("AZaz09")
			 )
		];


BOOST_SPIRIT_DEFINE(id);

}
}
}


#endif /* MW_TEST_PARSER_ID_HPP_ */
