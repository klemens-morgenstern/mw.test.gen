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
#include <mw/test/parser/comment.hpp>


namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class filename, std::string> const filename("filename");

auto const filename_def =
		no_skip[
		+(char_("-._$A-Za-z0-9")
		)];


auto file_lambda = [](auto& ctx)
		{
	std::cout << "Thingy129: " << _val(ctx) << std::endl;
	_val(ctx) = _attr(ctx);
		};


x3::rule<class file, std::string> const file("file");

auto const file_def =
		"file" >>
		filename[file_lambda] >> ";" ;


x3::rule<class tests_file, std::string> const tests_file("tests_file");

auto const tests_file_def =
		string("tests") >> "file" >> filename >> ";";

x3::rule<class include, std::string> const include("include");

auto const include_def = "include" >> filename >> ";";


BOOST_SPIRIT_DEFINE(filename, file, tests_file, include);



}
}
}


#endif /* MW_TEST_PARSER_FILE_HPP_ */
