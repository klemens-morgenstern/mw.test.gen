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
#include <mw/test/data/operations.hpp>

#include <iostream>

#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::code_check,
	(mw::test::data::check_qualification, qual)
	(mw::test::data::level_t, lvl)
	(mw::test::data::code, 	  data)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::throw_check,
	(bool, critical)
	(mw::test::data::code::iterator, 		_begin)
	(mw::test::data::level_t, 					 lvl)
	(mw::test::data::code_list,		 		exceptions)
	(std::vector<mw::test::data::check_entry>, steps)
	(mw::test::data::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::no_throw_check,
	(bool, critical)
	(mw::test::data::code::iterator, _begin)
	(mw::test::data::level_t, 				 lvl)
	(std::vector<mw::test::data::check_entry>, steps)
	(mw::test::data::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::any_throw_check,
	(bool, critical)
	(mw::test::data::code::iterator, _begin)
	(mw::test::data::level_t, 				 lvl)
	(std::vector<mw::test::data::check_entry>, steps)
	(mw::test::data::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::critical_section,
	(bool, critical)
	(mw::test::data::code::iterator, location)
	(std::vector<mw::test::data::check_entry>, steps)
	(mw::test::data::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::execute_check,
	(mw::test::data::code::iterator, location)
	(bool, 							 critical)
	(mw::test::data::level_t, 		 lvl)

);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::no_execute_check,
	(mw::test::data::code::iterator, location)
	(bool, 							 critical)
	(mw::test::data::level_t, 		 lvl)

);

namespace mw
{
namespace test
{
namespace parser
{

struct level_t : x3::symbols<data::level_t>
{
	level_t()
	{
		add	("assert", data::assertion)
			("expect", data::expectation)
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

x3::rule<class check_qualification, data::check_qualification> const check_qualification;


x3::rule<class func_content> 					 		 const func_content;
x3::rule<class execute_check, 	 data::execute_check>	 const execute_check;
x3::rule<class no_execute_check, data::no_execute_check> const no_execute_check;
x3::rule<class code_check, 		 data::code_check> 		 const code_check;
x3::rule<class crit_section, 	 data::critical_section> const crit_section;
x3::rule<class check_entry, 	 data::check_entry> 	 const check_entry;
x3::rule<class throw_check, 	 data::throw_check>		 const throw_check;
x3::rule<class no_throw_check, 	 data::no_throw_check>	 const no_throw_check;
x3::rule<class any_throw_check,	 data::any_throw_check>	 const any_throw_check;


x3::rule<class is_critical, bool> const is_critical;

auto const is_critical_def =
		  lit("critical")[l([](auto&c){_val(c) = true;})]
					| eps[l([](auto&c){_val(c) = false;})]
		;

auto const check_qualification_def =
		*omit[(	lit("static")  [oper::set_static  ]|
				lit("critical")[oper::set_critical]|
				lit("ranged")  [oper::set_ranged   ]|
				lit("bitwise") [oper::set_bitwise ])] ;


auto const execute_check_def =
		code_location >> is_critical >> level >> lit("execution") >> ";";

auto const no_execute_check_def =
		code_location >> is_critical >> level >> "no">> lit("execution") >> ";";

auto const code_check_def =
		check_qualification >> level >> code_chunk;

auto const throw_check_def =
			code_location
		>>  is_critical
		>>	level
		>> '(' >> code_list >>')'
		>>  code_section
		>> 	code_location ;

auto const no_throw_check_def =
			code_location
		>>  is_critical
		>>	level >> "no" >> "throw"
		>>  code_section
		>> 	code_location ;

auto const any_throw_check_def =
			code_location
		>>  is_critical
		>>	level >> "any" >> "throw"
		>>  code_section
		>> 	code_location ;

auto const crit_section_def =
		code_location >>
		'{' >> *check_entry >> '}'
		>> code_location;

auto const check_entry_def =
			execute_check
		|	no_execute_check
		|	code_check
		|	throw_check
		|	no_throw_check
		|	any_throw_check
		|	crit_section
		| 	code_chunk
		;

BOOST_SPIRIT_DEFINE(execute_check);
BOOST_SPIRIT_DEFINE(no_execute_check);

BOOST_SPIRIT_DEFINE(code_check);
BOOST_SPIRIT_DEFINE(crit_section);
BOOST_SPIRIT_DEFINE(check_entry);

BOOST_SPIRIT_DEFINE(throw_check);
BOOST_SPIRIT_DEFINE(no_throw_check);
BOOST_SPIRIT_DEFINE(any_throw_check);

BOOST_SPIRIT_DEFINE(is_critical);

BOOST_SPIRIT_DEFINE(check_qualification);

}
}
}



#endif /* MW_TEST_PARSER_OPERATIONS_HPP_ */
