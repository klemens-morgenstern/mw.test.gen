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
#include <mw/test/data/doc.hpp>
#include <type_traits>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


BOOST_FUSION_ADAPT_STRUCT(
		mw::test::data::doc,
		(std::string, head)
		(std::string, body)
);

namespace mw
{
namespace test
{
namespace parser
{

///general rule for a comment, used as a skipper
/** @code{.ebnf}
 *  <code> ::= <line_comment> | <block_comment> ;
 *  @endcode
 */
x3::rule<class comment> const comment("comment");

///Line comment parser
/** @code{.ebnf}
 *  <line_comment> ::= '//' (!eol)* eol ;
 *  @endcode
 */
x3::rule<class line_comment> 						const line_comment			("line_comment");

///Line comment before the entity in doxygen style, used for documentation
/** @code{.ebnf}
 *  <line_comment> ::= '//' ('/' | '!') (!eol)* eol ;
 *  @endcode
 */

x3::rule<class line_comment_pre_doc,  std::string>  const line_comment_pre_doc	("line_comment_pre_doc");

///Line comment after the entity in doxygen style, used for documentation
/** @code{.ebnf}
 *  <line_comment> ::= '//' ('/' | '!') '<' (!eol)* eol ;
 *  @endcode
 *
 */
x3::rule<class line_comment_post_doc, std::string> 	const line_comment_post_doc	("line_comment_post_doc");

auto const line_comment_def =
		"//" >> *(!eol >> char_) >> eol;

auto const line_comment_pre_doc_def =
		no_skip[omit[*(space | eol)] >>
				"//" >> +(lit('!') | '/') >> !lit('<') >> *(!eol >> char_) >> eol];

auto const line_comment_post_doc_def =
		no_skip[omit[*(space | eol)] >>
				"//" >> +(lit('!') | '/') >>  lit('<') >> *(!eol >> char_) >> eol];

BOOST_SPIRIT_DEFINE(line_comment_pre_doc, line_comment_post_doc);


/// General rule for a block comment used a skipper
/// @code
/// <block_comment> ::= '/*' (!'*/')* '*/' ;
/// @endcode
///
x3::rule<class block_comment> 						const block_comment			("block_comment");

/// Rule for a documenting block comment, before the entity
/// @code
/// <block_comment> ::= '/*' ('*' | '!') (!'*/')* '*/' ;
/// @endcode
///
x3::rule<class block_comment_pre_doc,  std::string> const block_comment_pre_doc	("block_comment_pre_doc");

/// Rule for a documenting block comment, after the entity
/// @code
/// <block_comment> ::= '/*' ('*' | '!') '<'  (!'*/')* '*/' ;
/// @endcode
///
x3::rule<class block_comment_post_doc, std::string> const block_comment_post_doc("block_comment_post_doc");

auto const block_comment_def =
		"/*" >> (*(!string("*/") >> char_)) >> "*/";

auto const block_comment_pre_doc_def =
		no_skip[omit[*(space | eol)] >>
				"/*" >> +(lit('!') | '*') >> !lit('<') >> *(!string("*/") >> char_) >>"*/"];

auto const block_comment_post_doc_def =
		no_skip[omit[*(space | eol)] >>
				"/*" >> +(lit('!') | '*') >>  lit('<') >> *(!string("*/") >> char_) >>"*/"];


BOOST_SPIRIT_DEFINE(block_comment_pre_doc, block_comment_post_doc);

auto const comment_def = line_comment | block_comment;

BOOST_SPIRIT_DEFINE(line_comment, block_comment, comment);


///Skipper rule
/** @code{.enbf}
 * <skiper> ::= <comment> | <end_of_line> | <space> ;
 * @endcode
 *
 */
x3::rule<class skipper> const skipper("skipper");

auto const skipper_def = comment | eol | space;

BOOST_SPIRIT_DEFINE(skipper);

///Pre entity comment
/** The first part of the doc, goes into the head member, the following ones into the body.
 * @code
 * <comment_pre_doc> ::= (<line_comment_pre_doc> | <block_comment_pre_doc> ) (<line_comment_pre_doc> | <block_comment_pre_doc> ) * ;
 * @endcode
 */
x3::rule<class comment_pre_doc, data::doc>  const comment_pre_doc ("comment_pre_doc");

auto const comment_pre_doc_def =
			 (line_comment_pre_doc | block_comment_pre_doc) >>
			*(line_comment_pre_doc | block_comment_pre_doc);

///Pre entity comment
/** The first part of the doc, goes into the head member, the following ones into the body.
 * @code
 * <comment_post_doc> ::= (<line_comment_post_doc> | <block_comment_post_doc> ) (<line_comment_post_doc> | <block_comment_post_doc> ) * ;
 * @endcode
 */
x3::rule<class comment_post_doc, data::doc> const comment_post_doc("comment_post_doc");

auto const comment_post_doc_def =
			 (line_comment_post_doc | block_comment_post_doc) >>
			*(line_comment_post_doc | block_comment_post_doc);


BOOST_SPIRIT_DEFINE(comment_pre_doc, comment_post_doc);

///Lambda used for documentation. It appends the string to an @ref mw::test::data::entity
auto const doc_f = [](auto &ctx)
{
	static_assert(
			std::is_convertible<decltype(_attr(ctx)), data::doc>::value,
			"doc_lambda must be used in a rule that yields a data::doc");

	_val(ctx).doc += _attr(ctx);

};

///This lambda allows to declare a rule as documented, which will provide the doc before and after to be added the entity.
auto doc = [](auto rule) {return -comment_pre_doc[doc_f] >> rule >> -comment_post_doc[doc_f]; };



}
}
}

#endif /* MW_TEST_COMMENT_HPP_ */
