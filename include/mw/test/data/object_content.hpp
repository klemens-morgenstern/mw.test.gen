/**
 * @file   object_content.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_OBJECT_CONTENT_HPP_
#define MW_TEST_DATA_OBJECT_CONTENT_HPP_


#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <mw/test/data/actions.hpp>
#include <mw/test/data/doc.hpp>
#include <mw/test/data/tpl_arg.hpp>
#include <mw/test/data/code.hpp>
#include <mw/test/data/operations.hpp>

namespace mw
{
namespace test
{
namespace data
{

struct obj_action
{
	doc_t doc;
	code::iterator location;
	action_t action;
	std::vector<data::check_entry> content;
};


struct using_plain
{
	doc_t doc;
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
};

struct using_throw
{
	doc_t doc;
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
	std::vector<std::string> exceptions;
};

struct using_no_throw
{
	doc_t doc;
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
};

struct using_any_throw
{
	code::iterator location;
	boost::optional<obj_id> id;
	action_t action;
};

struct using_decl  : boost::variant<using_plain, using_throw, using_no_throw, using_any_throw>
{
	doc_t doc;
};

}
}
}

#endif /* MW_TEST_PARSER_OBJECT_CONTENT_HPP_ */
