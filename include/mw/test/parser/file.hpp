/**
 * @file   file.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_FILE_HPP_
#define MW_TEST_PARSER_FILE_HPP_

#include <mw/test/parser/config.hpp>


namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class filename> const filename("filename");

auto const filename_def =
		+(char_("_") | char_("A", "Z") | char_("a", "z") | char_("0", "9") |
		  char_(".") | char_("-") | char_("$"));


x3::rule<class file> const file("file");

auto const file_def = "file" >> filename >> ";";


x3::rule<class tests_file> const tests_file("tests_file");

auto const tests_file_def =
		string("tests") >> "file" >> filename >> ";";

x3::rule<class include> const include("include");

auto const include_def = "include" >> filename >> ";";


BOOST_SPIRIT_DEFINE(filename, file, tests_file, include);



}
}
}


#endif /* MW_TEST_PARSER_FILE_HPP_ */
