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
x3::rule<class inheritance> const inheritance("inheritance");

auto const inheritance_def =
		":" >> (id >> -tpl_par) % ","
		;


x3::rule<class test_object> const test_object("test_object");

auto const test_object_def =
		string("test") >> "object" >> id
		>> -tpl_decl
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");


x3::rule<class composition> const composition("composition");

auto const composition_def =
		string("composition") >> id
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");


x3::rule<class classification> const classification("classification");

auto const classification_def =
		string("classification") >> id
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");

x3::rule<class class_> const class_("class_");

auto const class__def =
		string("class") >> id
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");


x3::rule<class test_case> const test_case("test_case");

auto const test_case_def =
		string("test") >> "case"  >> id
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");


x3::rule<class test_sequence> const test_sequence("test_sequence");

auto const test_sequence_def =
		string("test") >> "sequence"
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");


x3::rule<class test_step> const test_step("test_step");

auto const test_step_def =
		string("test") >> "step"
		>> -tpl_decl
		>> -inheritance
		>> "{"
		>> *object_content
		>> "}" >> -char_(";");





BOOST_SPIRIT_DEFINE(object_content);
BOOST_SPIRIT_DEFINE(inheritance);
BOOST_SPIRIT_DEFINE(test_object);
BOOST_SPIRIT_DEFINE(composition);
BOOST_SPIRIT_DEFINE(classification);
BOOST_SPIRIT_DEFINE(class_);
BOOST_SPIRIT_DEFINE(test_case);
BOOST_SPIRIT_DEFINE(test_sequence);
BOOST_SPIRIT_DEFINE(test_step);



}
}
}



#endif /* MW_TEST_PARSER_OBJECTS_HPP_ */
