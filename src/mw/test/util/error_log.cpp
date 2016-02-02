/**
 * @file   error_log.cpp
 * @date   18.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <mw/test/util/error_log.hpp>

namespace mw
{
namespace test
{
namespace util
{
static bool error_state = false;


void set_error()
{
	error_state = true;
}

void reset_error()
{
	error_state = false;
}

bool get_error()
{
	return error_state;
}


static bool _warn_as_error = false;

void set_warn_as_error(bool b)
{
	_warn_as_error = b;
}
bool warn_as_error()
{
	return _warn_as_error;
}

}
}
}
