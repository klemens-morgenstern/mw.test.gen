/**
 * @file   make_template.hpp
 * @date   18.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_FILL_TEMPLATE_HPP_
#define MW_TEST_FILL_TEMPLATE_HPP_

#include <mw/test/data/objects.hpp>
#include <mw/test/error_log.hpp>
#include <type_traits>

namespace mw
{
namespace test
{

template<typename Object, typename = std::is_base_of<data::object_tpl, Object>>
void fill_template(const data::object_tpl &tpl, const ast::obj_id& id)
{

	//ok, first do some error handling.
	if (tpl.tpl_args.size() < id.tpl_args.size())
		//Error(id.location.)

	std::vector<std::string> argl = tpl.tpl_args;//


}

}
}



#endif /* MW_TEST_TRANSFORM_MAKE_TEMPLATE_HPP_ */
