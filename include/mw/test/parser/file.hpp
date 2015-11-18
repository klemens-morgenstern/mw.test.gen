/**
 * @file   file.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_FILE_HPP_
#define MW_TEST_PARSER_FILE_HPP_


#include <mw/test/data/file.hpp>
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
		+(char_("-._$A-Za-z0-9"))];


auto file_lambda = [](auto& ctx)
		{
			data::entity::set_entity(_val(ctx));
			_val(ctx).filename = _attr(ctx);
		};



x3::rule<class file, data::file> const file("file");

auto const file_def =
			no_skip["file" >> (space | eol)]
					>> (filename  >> ";")[file_lambda] ;


x3::rule<class tests_file, data::tests_file> const tests_file("tests_file");

auto const tests_file_def =
			no_skip["tests"  >> (space | eol)]
		>> 	no_skip["file"   >> (space | eol)]
					>> (filename >> ";")[file_lambda];

x3::rule<class include, data::include> const include("include");

auto const include_def =
			no_skip["include" >> (space | eol)]
					>> (filename >> ";")[file_lambda];


BOOST_SPIRIT_DEFINE(filename, file, tests_file, include);



}
}
}


#endif /* MW_TEST_PARSER_FILE_HPP_ */
