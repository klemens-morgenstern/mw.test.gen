/**
 * @file   operations.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_OPERATIONS_HPP_
#define MW_TEST_PARSER_OPERATIONS_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/code.hpp>
#include <mw/test/parser/utility.hpp>
#include <mw/test/parser/actions.hpp>
#include <mw/test/ast/operations.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::code_check,
	(mw::test::ast::check_qualification, qual)
	(mw::test::ast::level_t, lvl)
	(mw::test::ast::code, 	  data)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::throw_check,
	(mw::test::ast::code::iterator, 		_begin)
	(bool, critical)
	(mw::test::ast::level_t, 				lvl)
	(mw::test::ast::code_list,		 		exceptions)
	(std::vector<mw::test::ast::check_entry>, steps)
	(mw::test::ast::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::no_throw_check,
	(mw::test::ast::code::iterator, _begin)
	(bool, critical)
	(mw::test::ast::level_t, 				 lvl)
	(std::vector<mw::test::ast::check_entry>, steps)
	(mw::test::ast::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::any_throw_check,
	(mw::test::ast::code::iterator, _begin)
	(bool, critical)
	(mw::test::ast::level_t, 				 lvl)
	(std::vector<mw::test::ast::check_entry>, steps)
	(mw::test::ast::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::critical_section,
	(mw::test::ast::code::iterator, _begin)
	(std::vector<mw::test::ast::check_entry>, steps)
	(mw::test::ast::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::execute_check,
	(mw::test::ast::code::iterator, location)
	(bool, 							 critical)
	(mw::test::ast::level_t, 		 lvl)

);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::no_execute_check,
	(mw::test::ast::code::iterator, location)
	(bool, 							 critical)
	(mw::test::ast::level_t, 		 lvl)

);

namespace mw
{
namespace test
{
namespace parser
{

struct level_t : x3::symbols<ast::level_t>
{
	level_t()
	{
		add	("assert", ast::assertion)
			("expect", ast::expectation)
			;
	}
} level;

namespace oper
{
auto set_static   = [](auto &ctx){_val(ctx).static_  = true;};
auto set_critical = [](auto &ctx){_val(ctx).critical = true;};
auto set_ranged	  = [](auto &ctx){_val(ctx).ranged 	 = true;};
auto set_bitwise  = [](auto &ctx){_val(ctx).bitwise  = true;};
auto set_data     = [](auto &ctx){_val(ctx).data = _attr(ctx);};
auto set_location = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx).location = itr;
		};
}

x3::rule<class check_qualification, ast::check_qualification> const check_qualification;


x3::rule<class execute_check, 	 ast::execute_check>	 const execute_check;
x3::rule<class no_execute_check, ast::no_execute_check> const no_execute_check;
x3::rule<class code_check, 		 ast::code_check> 		 const code_check;
x3::rule<class critical_section, ast::critical_section> const critical_section;

x3::rule<class check_entry, 	 ast::check_entry> 	 const check_entry;
x3::rule<class check_entries, 	 std::vector<ast::check_entry>> const check_entries;

x3::rule<class throw_check, 	 ast::throw_check>		 const throw_check;
x3::rule<class no_throw_check, 	 ast::no_throw_check>	 const no_throw_check;
x3::rule<class any_throw_check,	 ast::any_throw_check>	 const any_throw_check;

x3::rule<class execute_check_doc, 	 ast::execute_check>	 const execute_check_doc;
x3::rule<class no_execute_check_doc, ast::no_execute_check> const no_execute_check_doc;
x3::rule<class critical_section_doc, ast::critical_section> const critical_section_doc;

x3::rule<class check_entry_doc, 	 ast::check_entry> 	 const check_entry_doc;
x3::rule<class check_entries_doc, 	 std::vector<ast::check_entry>> const check_entries_doc;

x3::rule<class throw_check_doc, 	 ast::throw_check>		 const throw_check_doc;
x3::rule<class no_throw_check_doc, 	 ast::no_throw_check>	 const no_throw_check_doc;
x3::rule<class any_throw_check_doc,	 ast::any_throw_check>	 const any_throw_check_doc;



x3::rule<class is_critical, bool> const is_critical;

auto const is_critical_def =
		  lit("critical")[l([](auto&c){_val(c) = true;})]
					| eps[l([](auto&c){_val(c) = false;})]
		;

auto const check_qualification_def =
		*(	lit("static")  [oper::set_static  ] |
			lit("critical")[oper::set_critical] |
			lit("ranged")  [oper::set_ranged  ] |
			lit("bitwise") [oper::set_bitwise ] ) ;


auto const execute_check_def =
		code_location >> is_critical >> level >> lit("execution") >> ";";

auto const no_execute_check_def =
		code_location >> is_critical >> level >> "no">> lit("execution") >> ";";


auto const code_check_def =
		check_qualification >> level >> code_chunk;

auto const throw_check_def =
			code_location
		>>  is_critical
		>>	level >> "throw" >> '(' >> code_list >> ')'
		>>  '{' >> check_entries >> '}' >> -lit(';')
		>> 	code_location ;

auto const no_throw_check_def =
			code_location
		>>  is_critical
		>>	level >> "no" >> "throw"
		>>  '{' >> check_entries >> '}' >> -lit(';')
		>> 	code_location;

auto const any_throw_check_def =
			code_location
		>>  is_critical
		>>	level >> "any" >> "throw"
		>>  '{' >> check_entries >> '}' >> -lit(';')
		>> 	code_location ;

auto const check_entries_def = *check_entry ;


auto const critical_section_def =
		code_location >> "critical" >>
		'{' >> check_entries >> '}' >> -lit(';')
		>> code_location;


auto const execute_check_doc_def     = doc(execute_check    );
auto const no_execute_check_doc_def  = doc(no_execute_check );
auto const critical_section_doc_def  = doc(critical_section );
auto const check_entry_doc_def       = doc(check_entry      );
auto const check_entries_doc_def     = doc(check_entries    );
auto const throw_check_doc_def       = doc(throw_check	    );
auto const no_throw_check_doc_def    = doc(no_throw_check   );
auto const any_throw_check_doc_def   = doc(any_throw_check  );

auto const check_entry_def =
			execute_check_doc
		|	no_execute_check_doc
		|	code_check //< for some reason, this does not work with documentation.
		|	throw_check_doc
		|	no_throw_check_doc
		|	any_throw_check_doc
		|	critical_section_doc
		|   action_call
		| 	code_chunk;


BOOST_SPIRIT_DEFINE(execute_check);
BOOST_SPIRIT_DEFINE(no_execute_check);

BOOST_SPIRIT_DEFINE(code_check);

BOOST_SPIRIT_DEFINE(critical_section);

BOOST_SPIRIT_DEFINE(check_entry);
BOOST_SPIRIT_DEFINE(check_entries);

BOOST_SPIRIT_DEFINE(throw_check);
BOOST_SPIRIT_DEFINE(no_throw_check);
BOOST_SPIRIT_DEFINE(any_throw_check);

BOOST_SPIRIT_DEFINE(is_critical);

BOOST_SPIRIT_DEFINE(check_qualification);

BOOST_SPIRIT_DEFINE(execute_check_doc      );
BOOST_SPIRIT_DEFINE(no_execute_check_doc   );
BOOST_SPIRIT_DEFINE(critical_section_doc   );
BOOST_SPIRIT_DEFINE(check_entry_doc        );
BOOST_SPIRIT_DEFINE(check_entries_doc      );
BOOST_SPIRIT_DEFINE(throw_check_doc        );
BOOST_SPIRIT_DEFINE(no_throw_check_doc     );
BOOST_SPIRIT_DEFINE(any_throw_check_doc    );

}
}
}

#endif /* MW_TEST_PARSER_OPERATIONS_HPP_ */
