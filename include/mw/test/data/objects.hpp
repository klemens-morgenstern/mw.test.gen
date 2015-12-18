/**
 * @file   objects.hpp
 * @date   17.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_OBJECTS_HPP_
#define MW_TEST_DATA_OBJECTS_HPP_

#include <mw/test/data/object_content.hpp>

namespace mw
{
namespace test
{
namespace data
{

enum object_type_t
{
	object,
	classification,
	composition,
	class_,
	case_,
	step,
	sequence

};

struct test_object
{
	object_type_t type;
	obj_id id;
	std::vector<obj_id> inheritance;
	std::vector<object_content> content;
};

}
}
}



#endif /* MW_TEST_DATA_OBJECTS_HPP_ */
