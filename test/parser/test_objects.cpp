/**
 * @file   test_code.cpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */


#define BOOST_TEST_MODULE test_code
#include <boost/test/minimal.hpp>

#include <type_traits>

#include <mw/test/parsers/objects.hpp>
#include <mw/test/parsers/comment.hpp>



std::vector<mw::test::data::obj_id> req_id;
boost::typeindex::type_index req_type;
mw::test::data::location req_loc;
std::string req_make_id;

std::vector<mw::test::data::obj_id> req_inheritance;
std::vector<mw::test::data::object_content> req_obj_cont;
mw::test::data::code req_obj_tpl_cont;

std::vector<mw::test::data::tpl_arg> req_tpl_arg;

mw::test::data::object ret_object;
mw::test::data::object_tpl ret_tpl_object;


mw::test::data::doc_t pre_doc;
mw::test::data::doc_t post_doc;

// STUB the parser.cpp
namespace mw
{
namespace test
{

void parser::post_pre_doc (const data::doc_t & doc) {pre_doc = doc;}
void parser::post_post_doc(const data::doc_t & doc) {post_doc= doc;}

data::object& parser::make_object(
            const boost::typeindex::type_index & type,
            const data::location & loc,
            const std::string & id,
            const std::vector<data::obj_id> & inheritance,
            const std::vector<data::object_content> & obj_cont)
{
    req_type = type;
    req_loc = loc;
    req_make_id = id;
    req_inheritance = inheritance;
    req_obj_cont = obj_cont;
    return ret_object;
}

data::object_tpl& parser::register_template(
        const boost::typeindex::type_index & type,
        const data::location & loc,
        const std::string & id,
        const std::vector<data::tpl_arg> & tpl_arg,
        const std::vector<data::obj_id> & inheritance,
        const data::code & obj_cont)
{
    req_type = type;
    req_loc = loc;
    req_make_id = id;
    req_tpl_arg = tpl_arg;
    req_inheritance = inheritance;
    req_obj_tpl_cont = obj_cont;
    return ret_tpl_object;
}


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
    req_id.push_back(in);
    return nullptr;
}



} /* namespace test */
} /* namespace mw */


int test_main (int, char**)
{
    using std::cout;
    using std::endl;
    mw::test::parser parser;

    parser.include_stack.emplace(std::string(""));

    std::string &s = parser.include_stack.top().buffer;

    using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;

    namespace x3 = boost::spirit::x3;
    using namespace mw::test::parsers;

    namespace data = mw::test::data;
    std::string res;

    auto &beg = parser.include_stack.top()._begin;
    auto itr  = parser.include_stack.top()._begin;
    auto &end = parser.include_stack.top()._end;


	auto p = [&]()
		{
	        ret_object      = data::object();
	        ret_tpl_object  = data::object_tpl();
			beg = iterator(s.begin());
			itr = iterator(s.begin());
			end = iterator(s.end());
			return x3::phrase_parse(itr, end, test_object, skipper);
		};

    s = "testclass thingy {};";

	BOOST_CHECK(!p());

	s = "test_class thingy {} ;";

	BOOST_CHECK(p());
	BOOST_CHECK(itr == end);
	BOOST_CHECK(req_make_id == "thingy");
	cout << "req_make_id: '" << req_make_id << "'" << endl;

	BOOST_CHECK(req_obj_cont.size() == 0);
	cout << boost::core::demangle(req_type.name()) << endl,
	BOOST_CHECK(req_type == boost::typeindex::type_id<data::test_class>());


	s = "///Stuff\n"
	    "classification ding : xyz<42>, zyx { execute {} };";


	BOOST_CHECK(p());
	BOOST_CHECK(itr == end);
	BOOST_CHECK(req_make_id == "ding");
	BOOST_CHECK(req_obj_cont.size() == 1);
    BOOST_CHECK(req_type == boost::typeindex::type_id<data::test_classification>());
    BOOST_CHECK(pre_doc.head == "Stuff");
    std::cout << "XYZ: '" << ret_object.doc.head << "'" << std::endl;


    BOOST_REQUIRE(req_inheritance.size()             == 2);
    BOOST_CHECK  (req_inheritance[0].name            == "xyz");
    BOOST_REQUIRE(req_inheritance[0].tpl_args.size() == 1);
    BOOST_CHECK  (req_inheritance[0].tpl_args[0]     == "42");
    BOOST_CHECK  (req_inheritance[1].name            == "zyx");
    BOOST_CHECK  (req_inheritance[1].tpl_args.empty());


    s = "///Other Comment\n"
        "test_case some_tc<X, YZ=42> {...};";

    auto pl = [&]()
        {
            ret_object      = data::object();
            ret_tpl_object  = data::object_tpl();
            beg = iterator(s.begin());
            itr = iterator(s.begin());
            end = iterator(s.end());
            return x3::phrase_parse(itr, end, test_template, skipper);
        };

    BOOST_CHECK(pl());
    BOOST_CHECK(pre_doc.head == "Other Comment");
    BOOST_CHECK(req_make_id == "some_tc");
    BOOST_CHECK(req_obj_tpl_cont.content == "...");

    BOOST_REQUIRE(req_tpl_arg.size() == 2);
    BOOST_CHECK(req_tpl_arg[0].name == "X");
    BOOST_CHECK(!req_tpl_arg[0].default_arg);

    BOOST_CHECK(req_tpl_arg[1].name == "YZ");
    BOOST_REQUIRE(req_tpl_arg[1].default_arg);
    BOOST_CHECK(*req_tpl_arg[1].default_arg == "42");

	return 0;
}
