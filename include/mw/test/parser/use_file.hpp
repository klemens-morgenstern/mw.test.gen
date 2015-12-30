/**
 * @file   file.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_USE_FILE_HPP_
#define MW_TEST_PARSER_USE_FILE_HPP_


#include <mw/test/ast/use_file.hpp>
#include <mw/test/parser/config.hpp>
#include <mw/test/parser/comment.hpp>
#include <mw/test/parser/code.hpp>


BOOST_FUSION_ADAPT_STRUCT(
		mw::test::ast::use_file,
		(mw::test::ast::use_file_type,  type    )
		(mw::test::ast::code::iterator, location)
		(std::string, filename                  )
);

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
auto use_file_lambda = [](auto type)
		{
			return [type](auto & ctx)
					{
						_val(ctx) = type;
					};

		};

BOOST_SPIRIT_DEFINE(filename);


x3::rule<class use_file_type, ast::use_file_type> const use_file_type;

auto const use_file_type_def =
		(lexeme["tests"  >> skipper] >> lexeme["file" >> skipper])[use_file_lambda(ast::tests_file)] |
        (lexeme["file"   >> skipper])[use_file_lambda(ast::file)] |
		(lexeme["include">> skipper])[use_file_lambda(ast::include)];


BOOST_SPIRIT_DEFINE(use_file_type);


///Rule to parse mw::test::file
/** @code{.ebnf}
 *  <file> ::= 'file' <filename> ';' ;
 *  @endcode
 */
x3::rule<class use_file, ast::use_file> const use_file;

auto const use_file_def =
			use_file_type >>
			code_location >>
			filename >> ";";

BOOST_SPIRIT_DEFINE(use_file);


x3::rule<class use_file_doc, ast::use_file> const use_file_doc;

auto const use_file_doc_def = doc(use_file);


BOOST_SPIRIT_DEFINE(use_file_doc);



}
}
}


#endif /* MW_TEST_PARSER_USE_FILE_HPP_ */
