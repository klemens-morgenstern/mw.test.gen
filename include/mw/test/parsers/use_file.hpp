/**
 * @file   file.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_USE_FILE_HPP_
#define MW_TEST_PARSER_USE_FILE_HPP_


#include <mw/test/data/use_file.hpp>
#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/comment.hpp>
#include <mw/test/parsers/code.hpp>


BOOST_FUSION_ADAPT_STRUCT(
		mw::test::data::use_file,
		(mw::test::data::use_file_type,  type    )
		(mw::test::data::location, loc)
		(std::string, filename                  )
);

namespace mw
{
namespace test
{
namespace parsers
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

BOOST_SPIRIT_DEFINE(filename);


struct use_file_type_t : x3::symbols<data::use_file_type>
{
    use_file_type_t()
    {
        add ("file",       data::use_file_type::file)
            ("tests_file", data::use_file_type::tests_file)
            ("include",    data::use_file_type::include)
            ;
    }
} use_file_type;


///Rule to parse mw::test::file
/** @code{.ebnf}
 *  <file> ::= 'file' <filename> ';' ;
 *  @endcode
 */
x3::rule<class use_file, data::use_file> const use_file;

auto const use_file_def =
			lexeme[use_file_type >> skipper] >>
			code_location >>
			filename >> ";";

BOOST_SPIRIT_DEFINE(use_file);


x3::rule<class use_file_doc, data::use_file> const use_file_doc;

auto const use_file_doc_def = doc(use_file);


BOOST_SPIRIT_DEFINE(use_file_doc);



}
}
}


#endif /* MW_TEST_PARSER_USE_FILE_HPP_ */
