/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECT_CONTENT_HPP_
#define MW_TEST_PARSER_OBJECT_CONTENT_HPP_

#include <mw/test/ast/object_content.hpp>

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/operations.hpp>
#include <mw/test/parser/actions.hpp>
#include <mw/test/parser/comment.hpp>
#include <mw/test/parser/functional.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::obj_action,
	(mw::test::ast::code::iterator, location)
	(mw::test::ast::action_t, action)
	(std::vector<mw::test::ast::check_entry>, content)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::using_plain,
	(mw::test::ast::code::iterator, location)
	(boost::optional<mw::test::ast::obj_id>, id)
	(mw::test::ast::action_t, action)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::using_throw,
	(mw::test::ast::code::iterator, location)
	(boost::optional<mw::test::ast::obj_id>, id)
	(mw::test::ast::action_t, action)
	(mw::test::ast::level_t, level)
	(mw::test::ast::code_list, exceptions)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::using_no_throw,
	(mw::test::ast::code::iterator, location)
	(boost::optional<mw::test::ast::obj_id>, id)
	(mw::test::ast::action_t, action)
	(mw::test::ast::level_t, level)

);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::using_any_throw,
	(mw::test::ast::code::iterator, location)
	(boost::optional<mw::test::ast::obj_id>, id)
	(mw::test::ast::action_t, action)
	(mw::test::ast::level_t, level)

);


BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::using_decl,
	(decltype(mw::test::ast::using_decl::data), data)
);

namespace mw
{
namespace test
{
namespace parser
{

///Rule to parse
x3::rule<class obj_action, 		ast::obj_action> const obj_action;

auto const obj_action_def =
		code_location >> action >> '{' >> check_entries >> '}' >> -lit(';');


BOOST_SPIRIT_DEFINE(obj_action);


x3::rule<class obj_action_doc, 	ast::obj_action> const obj_action_doc;


auto const obj_action_doc_def = doc(obj_action);


BOOST_SPIRIT_DEFINE(obj_action_doc);



x3::rule<class using_plain,		ast::using_plain>		const using_plain;
x3::rule<class using_throw, 	ast::using_throw>	 	const using_throw;
x3::rule<class using_no_throw, 	ast::using_no_throw>	const using_no_throw;
x3::rule<class using_any_throw,	ast::using_any_throw> 	const using_any_throw;

x3::rule<class using_decl, ast::using_decl> const using_decl;


auto const using_plain_def =
			lexeme["using" >> code_location >> skipper ]
		>>	-((obj_id - action) >> "::")
		>> 	action
		>> 	";" ;

auto const using_throw_def =
		 	"using"
		>>  code_location
		>>  -((obj_id - action) >> "::")
		>>  action
		>> 	"for"
		>>	level >> "throw" >> '(' >> code_list >> ')' >> ";";

auto const using_no_throw_def =
	 		"using"
		>>  code_location
		>>  -((obj_id - action) >> "::")
		>>  action
		>>	"for"
		>>	level >> "no" >> "throw"
		>> ";";

auto const using_any_throw_def =
	 		"using"
		>>  code_location
		>>  -((obj_id - action) >> "::")
		>>  action
		>>	"for"
		>>	level >> "any" >> "throw"
		>> ";";



auto const using_decl_def =
			using_plain 	|
            using_throw 	|
            using_no_throw	|
            using_any_throw	;

x3::rule<class using_decl_doc, ast::using_decl> const using_decl_doc;

auto const using_decl_doc_def = doc(using_decl);

BOOST_SPIRIT_DEFINE(using_plain);
BOOST_SPIRIT_DEFINE(using_throw);
BOOST_SPIRIT_DEFINE(using_no_throw);
BOOST_SPIRIT_DEFINE(using_any_throw);
BOOST_SPIRIT_DEFINE(using_decl);
BOOST_SPIRIT_DEFINE(using_decl_doc);

x3::rule<class object_content, ast::object_content> const object_content;

auto const object_content_def = obj_action_doc | using_decl | functional | code_chunk | code_function ;

BOOST_SPIRIT_DEFINE(object_content);



}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
