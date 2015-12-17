/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECT_CONTENT_HPP_
#define MW_TEST_PARSER_OBJECT_CONTENT_HPP_

#include <mw/test/data/object_content.hpp>

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/operations.hpp>
#include <mw/test/parser/actions.hpp>
#include <mw/test/parser/comment.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::obj_action,
	(mw::test::data::code::iterator, location)
	(mw::test::data::action_t, action)
	(std::vector<mw::test::data::check_entry>, content)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::using_plain,
	(mw::test::data::code::iterator, location)
	(boost::optional<mw::test::data::obj_id>, id)
	(mw::test::data::action_t, action)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::using_throw,
	(mw::test::data::code::iterator, location)
	(boost::optional<mw::test::data::obj_id>, id)
	(mw::test::data::action_t, action)
	(std::vector<std::string>, exceptions)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::using_no_throw,
	(mw::test::data::code::iterator, location)
	(boost::optional<mw::test::data::obj_id>, id)
	(mw::test::data::action_t, action)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::using_any_throw,
	(mw::test::data::code::iterator, location)
	(boost::optional<mw::test::data::obj_id>, id)
	(mw::test::data::action_t, action)
);

namespace mw
{
namespace test
{
namespace parser
{

///Rule to parse
x3::rule<class obj_action, 		data::obj_action> const obj_action;

auto const obj_action_def =
		code_location >> action >> '{' >> check_entries >> '}' >> -lit(';');


BOOST_SPIRIT_DEFINE(obj_action);


x3::rule<class obj_action_doc, 	data::obj_action> const obj_action_doc;


auto const obj_action_doc_def = doc(obj_action);


BOOST_SPIRIT_DEFINE(obj_action_doc);



x3::rule<class using_plain,		data::using_plain>		const using_plain;
x3::rule<class using_throw, 	data::using_throw>	 	const using_throw;
x3::rule<class using_no_throw, 	data::using_no_throw>	const using_no_throw;
x3::rule<class using_any_throw,	data::using_any_throw> 	const using_any_throw;

x3::rule<class using_decl, data::using_decl> const using_decl;


auto const using_plain_def =
			lexeme["using" >> code_location >> skipper ]
		>>	-(id >> "::")
		>> 	action
		>> 	";" ;

auto const using_throw_def =
		 	"using"
		>>  code_location
		>>  -(id >> "::")
		>>  action
		>> 	"for"
		>>  is_critical
		>>	level >> "throw" >> '(' >> code_list >> ')' >> ";";

auto const using_no_throw_def =
	 		"using"
		>>  code_location
		>>  -(id >> "::")
		>>  action
		>>	"for"
		>>  is_critical
		>>	level >> "no" >> "throw"
		>> ";";

auto const using_any_throw_def =
	 		"using"
		>>  code_location
		>>  -(id >> "::")
		>>  action
		>>	"for"
		>>  is_critical
		>>	level >> "any" >> "throw"
		>> ";";



auto const using_decl_def =
		doc(using_plain 	|
            using_throw 	|
            using_no_throw	|
            using_any_throw	);

BOOST_SPIRIT_DEFINE(using_plain);
BOOST_SPIRIT_DEFINE(using_throw);
BOOST_SPIRIT_DEFINE(using_no_throw);
BOOST_SPIRIT_DEFINE(using_any_throw);
BOOST_SPIRIT_DEFINE(using_decl);

}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
