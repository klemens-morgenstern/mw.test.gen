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

namespace mw
{
namespace test
{
namespace data
{

struct equality
{
	std::string rhs;
	std::string lhs;
	std::string tolerance;
};

struct inequality
{
	std::string rhs;
	std::string lhs;
	std::string tolerance;
};


struct greater_equal
{
	std::string rhs;
	std::string lhs;
};

struct lesser_equal
{
	std::string rhs;
	std::string lhs;
};

struct greater
{
	std::string rhs;
	std::string lhs;
};

struct lesser
{
	std::string rhs;
	std::string lhs;
};

struct assignment
{
	std::string rhs;
	std::string lhs;
};






}
}
}


#endif /* MW_TEST_DATA_OPERATIONS_HPP_ */
