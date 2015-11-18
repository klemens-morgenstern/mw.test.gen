/**
 * @file   commend_doc.cpp
 * @date   17.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <mw/test/data/comment_doc.hpp>

namespace mw
{
namespace test
{
namespace data
{

void comment::add_before(const std::string& data)
{
	before += data;
}
void comment::add_behind (const std::string& data)
{
	behind += data;
}
void comment::clear()
{
	before.clear();
	behind.clear();
}

thread_local comment c;

comment& comment::get()
{
	return c;
}

bool comment::empty() const
{
	return before.empty() && behind.empty();
}

}
}
}

