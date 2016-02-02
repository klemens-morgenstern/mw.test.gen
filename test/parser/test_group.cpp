/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_group
#include <boost/test/minimal.hpp>

#include <mw/test/parsers/group.hpp>
#include <mw/test/parser.hpp>

mw::test::data::obj_id req_id;

// STUB the parser.cpp
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


data::object_p parser::get_object(const data::obj_id& in)
{
    req_id = in;
    return nullptr;
}


} /* namespace test */
} /* namespace mw */


int test_main (int, char**)
{
    mw::test::parser parser;

    parser.include_stack.emplace(std::string(""));

    std::string &s = parser.include_stack.top().buffer;

    using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

    namespace x3 = boost::spirit::x3;
    using namespace mw::test::parsers;


    auto &beg = parser.include_stack.top()._begin;
    auto itr  = parser.include_stack.top()._begin;
    auto &end = parser.include_stack.top()._end;

	namespace data = mw::test::data;
	data::group res;

	auto l = [&]()
		{
		 	 beg = iterator{ s.begin()};
		 	 itr = iterator{ s.begin()};
			 end = iterator{s.end()  };


			return x3::phrase_parse(itr, end, group, skipper, res);
		};

	s = "test_group main = {test1, /*zeugs*/test2<x,y,z,42>};";

	BOOST_CHECK(l());
	BOOST_CHECK(itr == end);


	return 0;
}
