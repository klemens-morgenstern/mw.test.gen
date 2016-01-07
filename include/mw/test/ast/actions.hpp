/**
 * @file   actions.hpp
 * @date   16.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_ACTIONS_HPP_
#define MW_TEST_AST_ACTIONS_HPP_

#include <string>
#include <vector>

namespace mw
{
namespace test
{
namespace ast
{

enum action_t
{
	evaluate,
	execute,
	initialize,
};

struct action_call
{
	action_t action;
	std::string id;
	std::vector<std::string> tpl_par;


};


}}}



#endif /* MW_TEST_AST_ACTIONS_HPP_ */
