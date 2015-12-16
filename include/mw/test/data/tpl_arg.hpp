/**
 * @file   tpl_decl.hpp
 * @date   21.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_TPL_ARG_HPP_
#define MW_TEST_DATA_TPL_ARG_HPP_


#include <string>
#include <mw/test/data/entity.hpp>

namespace mw
{
namespace test
{
namespace data
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



#endif /* MW_TEST_DATA_TPL_ARG_HPP_ */
