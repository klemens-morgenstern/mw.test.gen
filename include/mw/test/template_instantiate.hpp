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
#include <vector>
#include <memory>

#include <mw/test/ast/objects.hpp>
#include <mw/test/data/objects.hpp>

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
		std::string replace;
	};
	static std::string replace(const std::string & data, const std::vector<replacement> & replacements);

	std::function<data::object_tpl(const std::string &)> get_object;

	data::object_tpl_p make(const ast::test_object & to);
	data::object_tpl_p select_type(const ast::test_object & to);

};

} /* namespace test */
} /* namespace mw */

#endif /* TEMPLATE_INSTANTIATE_H_ */
