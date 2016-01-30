/**
 * @file   actions.hpp
 * @date   16.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 *
 *
 */

#ifndef MW_TEST_PARSER_ACTION_CALL_HPP_
#define MW_TEST_PARSER_ACTION_CALL_HPP_


#include <mw/test/data/action_call.hpp>

#include <mw/test/parsers/config.hpp>
#include <mw/test/parsers/id.hpp>
#include <mw/test/parsers/template.hpp>
#include <mw/test/parsers/comment.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::action_call,
	(mw::test::data::action_t, 	              type)
	(std::weak_ptr<mw::test::data::object>,   target)
);


namespace mw
{
namespace test
{
namespace parsers
{
///Symbol definition for actions in a test object.
/** @code{.ebnf}
 * 	<action> ::= 'init' | 'initialize'
 * 			 |	 'exec' | 'execute'
 * 			 |	 'eval' | 'evaluate' ;
 *  @endcode
 *
 */
struct action_t : x3::symbols<data::action_t>
{
	action_t()
	{
		add	("init", 	 	data::action_t::initialize)
			("initialize",  data::action_t::initialize)
			("exec", 	 	data::action_t::execute)
			("execute",  	data::action_t::execute)
			("eval", 	 	data::action_t::evaluate)
			("evaluate", 	data::action_t::evaluate)
			;
	}
} action;
///< Global variable of @ref action_t.


///< Rule for action_call, that is if a test_object wants to call another test objects action.
/** @code{.ebnf}
 *  <action_call> ::= <action> <id> [ <tpl_par_list> ] ';' ;
 *  @endcode
 *
 */
x3::rule<class action_call, data::action_call> action_call;

///Definition of @ref action_call.
auto const action_call_def =
        lexeme[action >> skipper] >> object_ref >> ';';

BOOST_SPIRIT_DEFINE(action_call);



}
}
}



#endif /* MW_TEST_PARSER_ACTIONS_HPP_ */
