/**
 * @file   test_runner.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_GROUP_HPP_
#define MW_TEST_PARSER_GROUP_HPP_

#include <mw/test/ast/group.hpp>
#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/id.hpp>
#include <mw/test/parsers/template.hpp>
#include <mw/test/parsers/code.hpp>

BOOST_FUSION_ADAPT_STRUCT(
		mw::test::ast::group,
		(mw::test::ast::code::iterator, location)
		(std::string, name)
		(std::vector<mw::test::ast::obj_id>, content)
);


namespace mw
{
namespace test
{
namespace parsers
{

x3::rule<class group, ast::group> const group;

auto const group_def =
		lexeme["test"  >> skipper] >> code_location >>
		lexeme["group" >> skipper] >>
		 id >> -lit("=") >>
		 "{" >>
		(obj_id % ',') >>
		"}" >> -lit(";");

BOOST_SPIRIT_DEFINE(group);

x3::rule<class group_doc, ast::group> const group_doc;
auto const group_doc_def = doc(group);
BOOST_SPIRIT_DEFINE(group_doc);


}
}
}
#endif /* MW_TEST_PARSER_GROUP_HPP_ */
