/**
 * @file   expression.hpp
 * @date   07.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_AST_EXPRESSIONS_HPP_
#define MW_TEST_AST_EXPRESSIONS_HPP_

#include <mw/test/ast/code.hpp>
#include <boost/optional.hpp>

namespace mw
{
namespace test
{
namespace ast
{

enum relativity_t
{
	absolute = 0, //default value
	relative,
	percent,
};

struct equality
{
	bool inverted; //true is '!=', false is '=='
	code lhs;
	code rhs;
	boost::optional<code> tolerance;
	relativity_t type;
};


enum compare_op
{
	greater,
	greater_equal,
	lesser,
	lesser_equal,
};

struct comparison
{
	code lhs;
	code rhs;
	compare_op oper;
};

struct predicate
{
	code name;
	code_list arg_list;
};


}
}
}



#endif /* MW_TEST_AST_EXPRESSIONS_HPP_ */
