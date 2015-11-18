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
#include <mw/test/data/comment_doc.hpp>
#include <iostream>

namespace mw
{
namespace test
{
namespace parser
{

auto comment_behind = [](auto &ctx) {data::comment::get().add_behind(_attr(ctx)); };
auto comment_before = [](auto &ctx) {data::comment::get().add_before(_attr(ctx)); };



x3::rule<class line_comment> const line_comment("line_comment");

auto const line_comment_def =
		no_skip["//" >>
				( (!char_("!/") >> *(!eol >> char_))
				| (+char_("!/") >>
				("<" 		  >> (*(!eol >> char_))[comment_behind]) |
				(+char_("!/") >> (*(!eol >> char_))[comment_before]) ))
			>> eol];

x3::rule<class block_comment> const block_comment("block_comment");

auto const block_comment_def =
		no_skip["/*" >>
			((!char_("!*") >> *(!string("*/") >> char_))
				|
			(+char_("!*") >>
				//(block_comment_doc_post | block_comment_doc)
				((+char_("<") >> (*(!string("*/") >> char_))[comment_behind]) |
				( !char_("<") >> (*(!string("*/") >> char_))[comment_before]))

			))
				] >> "*/" ;


x3::rule<class comment> const comment("comment");

auto const comment_def = line_comment | block_comment;

BOOST_SPIRIT_DEFINE(line_comment, block_comment, comment);

x3::rule<class skipper> const skipper("skipper");

auto const skipper_def = eol | space | comment ;

BOOST_SPIRIT_DEFINE(skipper);

}
}
}

#endif /* MW_TEST_COMMENT_HPP_ */
