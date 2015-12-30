/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_OBJECT_CONTENT_HPP_
#define MW_TEST_AST_OBJECT_CONTENT_HPP_


#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <mw/test/ast/actions.hpp>
#include <mw/test/ast/code.hpp>
#include <mw/test/ast/doc.hpp>
#include <mw/test/ast/functional.hpp>
#include <mw/test/ast/operations.hpp>
#include <mw/test/ast/tpl_arg.hpp>

namespace mw
{
namespace test
{
namespace ast
{

struct obj_action
{
	doc_t doc;
	code::iterator location;
	action_t action;
	std::vector<ast::check_entry> content;
};


struct using_plain
{
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
};

struct using_throw
{
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
	level_t level;
	code_list exceptions;
};

struct using_no_throw
{
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
	level_t level;
};

struct using_any_throw
{
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
	level_t level;
};

struct using_decl
{
	boost::variant<using_plain, using_throw, using_no_throw, using_any_throw> data;
	doc_t doc;
};

using object_content = boost::variant<obj_action, using_decl, functional, code>;

}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
