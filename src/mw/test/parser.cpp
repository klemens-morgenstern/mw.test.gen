/**
 * @file   parser.cpp
 * @date   29.01.2016
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <cassert>
#include <mw/test/parser.hpp>

namespace mw
{
namespace test
{

thread_local parser *_instance = nullptr;


parser::parser()
{
    assert(_instance == nullptr);

    _instance = this;
}


parser &parser::instance()
{
    assert(_instance != nullptr);
    return *_instance;
}

} /* namespace test */
} /* namespace mw */
