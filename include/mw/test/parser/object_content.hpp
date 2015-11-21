/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECT_CONTENT_HPP_
#define MW_TEST_PARSER_OBJECT_CONTENT_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/operations.hpp>

namespace mw
{
namespace test
{
namespace parser
{

///Rule to parse
x3::rule<class input> const input("input");
auto const input_def = "input" >> char_("{") >> func_content >> "}";

x3::rule<class output> const output("output");
auto const output_def = "output" >> char_("{") >> func_content >> "}";

x3::rule<class execute> const execute("output");
auto const executet_def = "execute" >> char_("{") >> func_content >> "}";



x3::rule<class object_content> const object_content("object_content");

auto const object_content_def =
		code   |
		input  |
		output |
		execute;



BOOST_SPIRIT_DEFINE(object_content, input, output, execute);


}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
