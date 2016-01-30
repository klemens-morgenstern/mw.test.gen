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


//using plain adapt.
BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::object_action,
        (mw::test::data::location, location)
        (mw::test::data::action_t, action)
        (std::vector<mw::test::data::check_entry>, content)
);


//using plain adapt.
BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::using_decl,
        (mw::test::data::location, location)
        (std::weak_ptr<mw::test::data::object>, obj)
        (mw::test::data::action_t, action)
);


//using throw adapt.
BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::using_decl,
        (mw::test::data::location,  location)
        (std::weak_ptr<mw::test::data::object>, obj)
        (mw::test::data::action_t,  action)
        (mw::test::data::level_t,   level)
        (boost::optional<mw::test::data::code_list>, content)
);

//using any/no throw adapt.
BOOST_FUSION_ADAPT_STRUCT(
        mw::test::data::using_decl,
        (mw::test::data::location,  location)
        (std::weak_ptr<mw::test::data::object>, obj)
        (mw::test::data::action_t,  action)
        (mw::test::data::level_t,   level)
);


namespace mw
{
namespace test
{
namespace parsers
{

///Rule to parse
x3::rule<class obj_action, 		data::object_action> const obj_action;

auto const obj_action_def =
		code_location >> action >> '{' >> check_entries >> '}' >> -lit(';');


BOOST_SPIRIT_DEFINE(obj_action);


x3::rule<class obj_action_doc, 	data::object_action> const obj_action_doc;


auto const obj_action_doc_def = doc(obj_action);


BOOST_SPIRIT_DEFINE(obj_action_doc);

/*
struct using_adapt
{
    //using_plain
    data::using_decl handle(const std::tuple<data::location, data::obj_id, data::action_t> & tpl)
    {
        data::using_decl ud;
        ud.loc    = std::get<0>(tpl);
        ud.obj    = parser::instance().get_object(std::get<1>(tpl));
        ud.type   = data::using_type::plain;
        ud.action = std::get<2>(tpl);
        return std::move(ud);
    }
    //using_throw
    data::using_decl handle(const std::tuple<data::location, data::obj_id,
                                  data::action_t, data::level, data::code_list> & tpl)
    {
        data::using_decl ud;
        ud.loc    = std::get<0>(tpl);
        ud.obj    = parser::instance().get_object(std::get<1>(tpl));
        ud.type   = data::using_type::throw_;
        ud.action = std::get<2>(tpl);
        ud.lvl    = std::get<3>(tpl);
        ud.exceptions = std::get<4>(tpl);
        return std::move(ud);
    }

    void operator()(auto in) {_val(in) = handle(_attr(in)); }
};*/

struct using_type_t : x3::symbols<data::using_type>
{
    using_type_t()
    {
        add ("throw",    data::using_type::throw_)
            ("no_throw", data::using_type::no_throw)
            ("any_throw",data::using_type::any_throw);
    }
} using_type;


x3::rule<class using_plain,		data::using_decl>	const using_plain;
x3::rule<class using_throw, 	data::using_decl>	const using_throw;
x3::rule<class using_no_throw, 	data::using_decl>	const using_no_throw;
x3::rule<class using_any_throw,	data::using_decl> 	const using_any_throw;

x3::rule<class using_decl, data::using_decl> const using_decl;


auto const using_plain_def =
			lexeme["using" >> code_location >> skipper ]
		>>	obj_id >> "::"
		>> 	action
		>> 	";" ;

auto const using_throw_def =
		 	"using"
		>>  code_location
		>>  obj_id >> "::"
		>>  action
		>> 	"for"
		>>	level >> using_type >> '(' >> code_list >> ')' >> ";";

auto const using_no_throw_def =
	 		"using"
		>>  code_location
		>>  obj_id  >> "::"
		>>  action
		>>	"for"
		>>	level >> using_type
		>> ";";

auto const using_any_throw_def =
	 		"using"
		>>  code_location
		>>  obj_id >> "::"
		>>  action
		>>	"for"
		>>	level >> using_type
		>> ";";



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
