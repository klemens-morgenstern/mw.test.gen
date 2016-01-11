/**
 * @file   objects.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECTS_HPP_
#define MW_TEST_PARSER_OBJECTS_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/parser/template.hpp>
#include <mw/test/parser/object_content.hpp>
#include <mw/test/ast/objects.hpp>


BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::test_object,
	(mw::test::ast::object_type_t, 				 type)
	(mw::test::ast::obj_id, 					 id)
	(std::vector<mw::test::ast::tpl_arg>,		 tpl_args)
	(std::vector<mw::test::ast::obj_id>, 		 inheritance)
	(std::vector<mw::test::ast::object_content>, content)
);

namespace mw
{
namespace test
{
namespace parser
{


///Rule to declare the inheritance of an object
/**
 * @code{.ebnf}
 * <inheritance> ::= ':' <id> -<tpl_par> (<id> -<tpl_par>)* ;
 * @endcode
 */
x3::rule<class inheritance, std::vector<ast::obj_id>> const inheritance;

auto const inheritance_def =
		-(":" >> obj_id % ",")
		;

BOOST_SPIRIT_DEFINE(inheritance);

struct plain_obj_type_t : x3::symbols<ast::object_type_t>
{
	plain_obj_type_t()
	{
		add	("classification",  ast::classification)
			("composition", 	ast::composition)
			;
	}
} plain_obj_type;


struct test_obj_type_t : x3::symbols<ast::object_type_t>
{
	test_obj_type_t()
	{
		add ("object", ast::object)
			("class",  ast::class_)
			("case",   ast::case_)
			("step",   ast::step)
			("sequence", ast::sequence);
	}
} test_obj_type;


x3::rule<class object_type, ast::object_type_t> const object_type;

auto const object_type_def =
		plain_obj_type |
		(lexeme["test" >> skipper] >> test_obj_type) ;

BOOST_SPIRIT_DEFINE(object_type);



x3::rule<class test_object, ast::test_object> const test_object;

auto const test_object_def =
		object_type
		>> obj_id
		>> -tpl_decl
		>> inheritance
		>> "{"
		>> *object_content
		>> lit("}") >> -lit(";");

BOOST_SPIRIT_DEFINE(test_object);

x3::rule<class test_object_doc, ast::test_object> const test_object_doc;
auto const test_object_doc_def = doc(test_object);

BOOST_SPIRIT_DEFINE(test_object_doc);


}
}
}



#endif /* MW_TEST_PARSER_OBJECTS_HPP_ */
