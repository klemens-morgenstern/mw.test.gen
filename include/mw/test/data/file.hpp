/**
 * @file   file.hpp
 * @date   18.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_FILE_HPP_
#define MW_TEST_DATA_FILE_HPP_

#include <string>
#include <mw/test/data/entity.hpp>

namespace mw
{
namespace test
{
namespace data
{
///Base class for objects including a filename.
struct file_decl : entity
{
	std::string filename;
};

struct file		  : file_decl {}; ///< a file, used by a test file, that is anything that is tested
struct include 	  : file_decl {}; ///< a test-defintion file, included by this file
struct tests_file : file_decl {}; ///< explicit statement that this tests tests a file.

}
}
}



#endif /* MW_TEST_DATA_FILE_HPP_ */
