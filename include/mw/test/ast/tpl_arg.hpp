/**
 * @file   tpl_decl.hpp
 * @date   21.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_TPL_ARG_HPP_
#define MW_TEST_AST_TPL_ARG_HPP_


#include <mw/test/ast/entity.hpp>
#include <string>

namespace mw
{
namespace test
{
namespace ast
{
///Base class for objects including a filename.
struct tpl_arg
{
	std::string name;
	std::string def_arg;
};

///object id, may also contain template parameters
struct obj_id
{
	std::string name;
	std::vector<std::string> tpl_args;
};

}
}
}



#endif /* MW_TEST_AST_TPL_ARG_HPP_ */
