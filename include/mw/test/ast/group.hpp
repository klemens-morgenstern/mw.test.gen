/**
 * @file   test_group.hpp
 * @date   29.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_GROUP_HPP_
#define MW_TEST_AST_GROUP_HPP_

#include <vector>
#include <mw/test/ast/tpl_arg.hpp>
#include <mw/test/ast/code.hpp>
#include <mw/test/parser/comment.hpp>

namespace mw
{
namespace test
{
namespace ast
{

struct group
{
	code::iterator location;
	std::string name;
	std::vector<obj_id> content;
	doc_t doc;
};

}
}
}



#endif /* MW_TEST_AST_GROUP_HPP_ */
