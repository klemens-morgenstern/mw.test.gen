/**
 * @file   comment.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_COMMENT_HPP_
#define MW_TEST_COMMENT_HPP_

#include <mw/test/parser/config.hpp>

namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class line_comment> const line_comment("line_comment");

auto const line_comment_def =
		no_skip["//" >> !char_("/") >> *(!eol >> char_) >> eol];


x3::rule<class block_comment> const block_comment("block_comment");

auto const block_comment_def =
		no_skip["/*" >> !char_("*") >>*(!string("*/") >> char_) >> "*/"] ;

x3::rule<class comment> const comment("comment");

auto const comment_def = line_comment | block_comment;



x3::rule<class line_comment_doc> const line_comment_doc("line_comment");

auto const line_comment_doc_def =
		("///" | string("//!")) >> no_skip[*!eol >> eol];


x3::rule<class block_comment_doc> const block_comment_doc("block_comment");

auto const block_comment_doc_def =
		"/**" >> no_skip[*-string("*/") >> "*/"];

x3::rule<class comment_doc> const comment_doc("comment_doc");

auto const comment_doc_def = line_comment_doc | block_comment_doc;



BOOST_SPIRIT_DEFINE(line_comment, 	  block_comment, 	 comment);
BOOST_SPIRIT_DEFINE(line_comment_doc, block_comment_doc, comment_doc);

x3::rule<class skip> const skip("skip");

auto const skip_def = eol | space | comment | comment_doc;

BOOST_SPIRIT_DEFINE(skip);

}
}
}

#endif /* MW_TEST_COMMENT_HPP_ */
