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

///Defintion for an id
/** @return std::string
 * @code{.enbf}
 * <id> ::= '[_A-Za-z]' '[_A-Za-z0-9]'* ;
 * @endcode
 */
x3::rule<class id, std::string> const id("id");

auto const id_def =
		lexeme[
			char_("_A-Za-z") >> *char_("_A-Za-z0-9")
		];


BOOST_SPIRIT_DEFINE(id);

}
}
}


#endif /* MW_TEST_PARSER_ID_HPP_ */
