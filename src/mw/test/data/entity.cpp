/**
 * @file   entity.cpp
 * @date   18.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <mw/test/data/entity.hpp>

namespace mw
{
namespace test
{
namespace data
{

thread_local entity *e_buf = nullptr;
thread_local std::string doc_buf;

void entity::set_entity(entity & e)
{
	e.doc += doc_buf;
	doc_buf.clear();
	e_buf = &e;
}
void entity::add_post(const std::string& data)
{
	if (e_buf != nullptr)
		e_buf->doc += data;
}
void entity::add_pre(const std::string& data)
{
	doc_buf += data;
}


}
}
}
