/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECT_CONTENT_HPP_
#define MW_TEST_PARSER_OBJECT_CONTENT_HPP_


#include <tuple>
#include <mw/test/data/object_content.hpp>

#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/id.hpp>
#include <mw/test/parsers/code.hpp>
#include <mw/test/parsers/operations.hpp>
#include <mw/test/parsers/actions.hpp>
#include <mw/test/parsers/comment.hpp>
#include <mw/test/parsers/functional.hpp>
#include <mw/test/parsers/template.hpp>

#include <boost/fusion/include/adapt_struct.hpp>


BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::object_action,
        (mw::test::data::location, loc)
        (mw::test::data::action_t, action)
        (std::vector<mw::test::data::check_entry>, content)
);

BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::object_content,
        (mw::test::data::object_content::variant, data)
);

namespace mw
{
namespace test
{
namespace parsers
{

auto make_using_plain = [](auto & ctx)
        {
            using boost::fusion::at_c;
            data::using_decl & val = x3::_val(ctx);
            auto & c = x3::_attr(ctx);
            val.type    = data::using_type::plain;
            val.loc     = at_c<0>(c);
            val.obj     =
                    parser::instance().get_object(at_c<1>(c));
            val.action  = at_c<2>(c);
        };

auto make_using_throw = [](auto & ctx)
        {
            using boost::fusion::at_c;
            data::using_decl & val = x3::_val(ctx);
            auto & c = x3::_attr(ctx);
            val.type    = data::using_type::throw_;
            val.loc         = at_c<0>(c);
            val.obj         =
                parser::instance().get_object(at_c<1>(c));
            val.action      = at_c<2>(c);
            val.level       = at_c<3>(c);
            val.exceptions  = at_c<4>(c);
        };

auto make_using_no_throw = [](auto & ctx)
        {
            using boost::fusion::at_c;
            data::using_decl & val = x3::_val(ctx);
            auto & c = x3::_attr(ctx);
            val.type    = data::using_type::no_throw;

            val.loc         = at_c<0>(c);
            val.obj         =
                    parser::instance().get_object(at_c<1>(c));
            val.action      = at_c<2>(c);
            val.level       = at_c<3>(c);
        };

auto make_using_any_throw = [](auto & ctx)
        {
            using boost::fusion::at_c;
            data::using_decl & val = x3::_val(ctx);
            auto & c = x3::_attr(ctx);
            val.type    = data::using_type::any_throw;

            val.loc         = at_c<0>(c);
            val.obj         =
                    parser::instance().get_object(at_c<1>(c));
            val.action      = at_c<2>(c);
            val.level       = at_c<3>(c);
        };

///Rule to parse
x3::rule<class obj_action, 		data::object_action> const obj_action;

auto const obj_action_def =
		code_location >> action >> '{' >> check_entries >> '}' >> -lit(';');


BOOST_SPIRIT_DEFINE(obj_action);


x3::rule<class obj_action_doc, 	data::object_action> const obj_action_doc;


auto const obj_action_doc_def = doc(obj_action);


BOOST_SPIRIT_DEFINE(obj_action_doc);


x3::rule<class using_plain,		data::using_decl>	const using_plain;
x3::rule<class using_throw, 	data::using_decl>	const using_throw;
x3::rule<class using_no_throw, 	data::using_decl>	const using_no_throw;
x3::rule<class using_any_throw,	data::using_decl> 	const using_any_throw;

x3::rule<class using_decl, data::using_decl> const using_decl;


auto const using_plain_def =
			(lexeme["using" >> code_location >> skipper ]
		>>	obj_id >> "::"
		>> 	action
		>> 	";")[make_using_plain] ;

auto const using_throw_def =
		 	("using"
		>>  code_location
		>>  obj_id >> "::"
		>>  action
		>> 	"for"
		>>	level >> "throw" >> '(' >> code_list >> ')' >> ";")[make_using_throw];

auto const using_no_throw_def =
	 		("using"
		>>  code_location
		>>  obj_id  >> "::"
		>>  action
		>>	"for"
		>>	level >> "no_throw"
		>> ";")[make_using_no_throw];

auto const using_any_throw_def =
	 		("using"
		>>  code_location
		>>  obj_id >> "::"
		>>  action
		>>	"for"
		>>	level >> "any_throw"
		>> ";")[make_using_any_throw];



auto const using_decl_def =
			using_plain 	|
            using_throw 	|
            using_no_throw	|
            using_any_throw	;

x3::rule<class using_decl_doc, data::using_decl> const using_decl_doc;

auto const using_decl_doc_def = doc(using_decl);

BOOST_SPIRIT_DEFINE(using_plain);
BOOST_SPIRIT_DEFINE(using_throw);
BOOST_SPIRIT_DEFINE(using_no_throw);
BOOST_SPIRIT_DEFINE(using_any_throw);
BOOST_SPIRIT_DEFINE(using_decl);
BOOST_SPIRIT_DEFINE(using_decl_doc);

x3::rule<class object_content, data::object_content> const object_content;

auto const object_content_def = obj_action_doc | using_decl | functional | code_chunk | code_function ;

BOOST_SPIRIT_DEFINE(object_content);



}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
