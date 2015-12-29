/**
 * @file   file.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_FILE_HPP_
#define MW_TEST_PARSER_FILE_HPP_


#include <mw/test/ast/file.hpp>
#include <mw/test/parser/config.hpp>
#include <mw/test/parser/comment.hpp>

namespace mw
{
namespace test
{
namespace parser
{
/** Rule to parase a filename
 * @code{.ebnf}
 * <filename> ::= '[-.$A-Za-z0-9]+' ;
 * @endcode
 * Has a pre-skip.
 *
 */
x3::rule<class filename, std::string> const filename;

auto const filename_def =
		lexeme[
		+(char_("-._$A-Za-z0-9"))];

///Lambda to set the filename of an object with a filename member. Mostly used when the object is inheriting mw::test::data::file_decl
auto file_lambda = [](auto& ctx)
		{
			_val(ctx).filename = _attr(ctx);
		};


///Rule to parse mw::test::file
/** @code{.ebnf}
 *  <file> ::= 'file' <filename> ';' ;
 *  @endcode
 */
x3::rule<class file, ast::file> const file;

auto const file_def =
			doc(
			   lexeme["file" >> (space | eol)]
			>> (filename  >> ";")[file_lambda]);

///Rule to parse mw::test::tests_file
/** @code{.ebnf}
 *  <file> ::= 'tests' 'file' <filename> ';' ;
 *  @endcode
 */
x3::rule<class tests_file, ast::tests_file> const tests_file;

auto const tests_file_def =
		doc(
			lexeme["tests"  >> (space | eol)]
		>> 	lexeme["file"   >> (space | eol)]
		>> (filename >> ";")[file_lambda]);


///Rule to parse mw::test::include
/** @code{.ebnf}
 *  <file> ::= 'include' <filename> ';' ;
 *  @endcode
 */
x3::rule<class include, ast::include> const include;

auto const include_def =
		doc(lexeme["include" >> (space | eol)]
		>> (filename >> ";")[file_lambda]);


BOOST_SPIRIT_DEFINE(filename, file, tests_file, include);



}
}
}


#endif /* MW_TEST_PARSER_FILE_HPP_ */
