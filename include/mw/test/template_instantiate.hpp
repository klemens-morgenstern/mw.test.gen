/**
 * @file   template_instantiate.h
 * @date   19.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_TEMPLATE_INSTANTIATE_H_
#define MW_TEST_TEMPLATE_INSTANTIATE_H_

#include <string>
#include <regex>

namespace mw {
namespace test {

/*
 *
 */
struct template_instantiate
{
	struct replacement
	{
		std::string id;
		std::string replacement;
	};

	static std::string replace(const std::string & data, const std::vector<replacement> & replacements);
};

} /* namespace test */
} /* namespace mw */

#endif /* TEMPLATE_INSTANTIATE_H_ */
