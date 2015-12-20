/**
 * @file   call_trace.hpp
 * @date   19.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_FUNCTIONAL_HPP_
#define MW_TEST_DATA_FUNCTIONAL_HPP_


#include <mw/test/data/code.hpp>

namespace mw
{
namespace test
{
namespace data
{

struct call_trace
{
	code name;
	boost::optional<std::vector<call_trace>> content{};
	boost::optional<unsigned int> count{};
};


struct call_trace_decl : call_trace
{
	std::string id;
};

struct stub
{
	code name;
	boost::optional<std::string> return_name;
	std::vector<std::string> arg_names;
};

struct fake
{
	code name;
	code func;
};


}
}
}



#endif /* MW_TEST_PARSER_CALL_TRACE_HPP_ */
