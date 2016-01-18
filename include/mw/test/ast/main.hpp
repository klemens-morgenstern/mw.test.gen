/**
 * @file   main.hpp
 * @date   29.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_MAIN_HPP_
#define MW_TEST_AST_MAIN_HPP_

#include <mw/test/ast/objects.hpp>
#include <mw/test/ast/group.hpp>
#include <mw/test/ast/use_file.hpp>

namespace mw
{
namespace test
{
namespace ast
{

struct main
{
	std::vector<use_file> 	 use_files;
	std::vector<group> 		 groups;
	std::vector<test_object> test_objects;
};


}





}}



#endif /* MW_TEST_AST_MAIN_HPP_ */
