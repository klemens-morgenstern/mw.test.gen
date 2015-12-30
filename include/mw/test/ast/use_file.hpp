/**
 * @file   file.hpp
 * @date   18.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_USE_FILE_HPP_
#define MW_TEST_AST_USE_FILE_HPP_

#include <mw/test/ast/doc.hpp>
#include <mw/test/ast/code.hpp>
#include <string>

namespace mw
{
namespace test
{
namespace ast
{

enum use_file_type
{
	tests_file,
	file,
	include,
};

///Base class for objects including a filename.
struct use_file
{
	doc_t doc;
	code::iterator location;
	use_file_type type;
	std::string filename;
};

}
}
}

#endif /* MW_TEST_AST_USE_FILE_HPP_ */
