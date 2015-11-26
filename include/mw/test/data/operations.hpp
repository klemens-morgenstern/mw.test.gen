/**
 * @file   code.hpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_OPERATIONS_HPP_
#define MW_TEST_DATA_OPERATIONS_HPP_

#include <vector>
#include <string>

namespace mw
{
namespace test
{
namespace data
{

class operation
{
	bool static_   = false;
	bool bitwise  = false;
	bool range    = false;
	bool critical = false;
	std::string data{};
};

struct assertion 	: operation {};
struct expectation 	: operation {};






}
}
}


#endif /* MW_TEST_DATA_OPERATIONS_HPP_ */
