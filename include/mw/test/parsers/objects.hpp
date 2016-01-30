/**
 * @file   objects.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OBJECTS_HPP_
#define MW_TEST_PARSER_OBJECTS_HPP_

#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/id.hpp>
#include <mw/test/parsers/template.hpp>
#include <mw/test/parsers/object_content.hpp>
#include <mw/test/parsers/code.hpp>
#include <mw/test/data/objects.hpp>


namespace mw
{
namespace test
{
namespace parsers
{

///Rule to declare the inheritance of an object
/**
 * @code{.ebnf}
 * <inheritance> ::= ':' <id> -<tpl_par> (<id> -<tpl_par>)* ;
 * @endcode
 */
x3::rule<class inheritance, std::vector<data::obj_id>> const inheritance;

auto const inheritance_def =
		-(":" >> obj_id % ",")
		;

BOOST_SPIRIT_DEFINE(inheritance);


struct object_type_t : x3::symbols<boost::typeindex::type_index>
{
    object_type_t()
	{
		add ("classification",  boost::typeindex::type_id<data::test_classification>())
            ("composition",     boost::typeindex::type_id<data::test_composition>())
            ("test_object",     boost::typeindex::type_id<data::test_object>())
			("test_class",      boost::typeindex::type_id<data::test_class>())
			("test_case",       boost::typeindex::type_id<data::test_case>())
			("test_step",       boost::typeindex::type_id<data::test_step>())
			("test_sequence",   boost::typeindex::type_id<data::test_sequence>())
			;
	}
} object_type;


auto const make_object = [](auto & c)
        {
            auto &t = x3::_attr(c);
            using boost::fusion::at_c;
            x3::_val(c) = //used to add doc
                parser::instance().make_object(
                    at_c<0>(t), //type_index
                    at_c<1>(t), //location
                    at_c<2>(t), //id
                    at_c<3>(t), //tpl-decls
                    at_c<4>(t), //inheritance
                    at_c<5>(t) //object_content
                    );

        };

x3::rule<class test_object, data::object&> const test_object;

auto const test_object_def =
        object_type
		>> code_location
		>> id
		>> -tpl_decl
		>> inheritance
		>> "{"
		>> *object_content
		>> lit("}") >> -lit(";");

BOOST_SPIRIT_DEFINE(test_object);

x3::rule<class test_object_doc, data::test_object> const test_object_doc;
auto const test_object_doc_def = doc(test_object);

BOOST_SPIRIT_DEFINE(test_object_doc);


}
}
}



#endif /* MW_TEST_PARSER_OBJECTS_HPP_ */
