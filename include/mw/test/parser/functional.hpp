/**
 * @file   call_trace.hpp
 * @date   19.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_FUNCTIONAL_HPP_
#define MW_TEST_PARSER_FUNCTIONAL_HPP_

#include <mw/test/parser/code.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <mw/test/data/functional.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::call_trace,
	(mw::test::data::code, name)
	(boost::optional<std::vector<mw::test::data::call_trace>>, content)
	(boost::optional<unsigned int>,  count)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::call_trace_decl,
	(std::string, id)
	(mw::test::data::code, name)
	(boost::optional<std::vector<mw::test::data::call_trace>>, content)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::stub,
	(mw::test::data::code, name)
	(boost::optional<std::string>, return_name)
	(std::vector<std::string>, arg_names)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::data::fake,
	(mw::test::data::code, name)
	(mw::test::data::code, func)
);

namespace mw
{
namespace test
{
namespace parser
{

x3::rule<class function_name, data::code> const function_name;
auto const function_name_def = eps[code::set_beg] >>
		omit[lexeme[+(!(lit(',') | "=>" ) >> code_chunk_step)]]
		  >> eps[code::set_end];;

x3::rule<class call_trace, data::call_trace> const call_trace;

x3::rule<class call_trace_list, std::vector<data::call_trace>> const call_trace_list;


auto const call_trace_list_def =
		(call_trace % ",") |
		eps ;


auto const call_trace_def =
		function_name
		>> -("=>" >> lit("{")
		>> call_trace_list
		>> "}")
		>> -('*' >> x3::uint_);



x3::rule<class call_trace_decl, data::call_trace_decl> const call_trace_decl;
auto const call_trace_decl_def =
		lexeme["call" 	>> skipper] >>
		lexeme["trace" 	>> skipper] >>
		lexeme[id  	 >> skipper] >>
		lexeme["for" >> skipper] >>
		function_name >> "=>"  >>lit("{")
		>> call_trace_list
		>> "}" >> ";";

BOOST_SPIRIT_DEFINE(function_name);
BOOST_SPIRIT_DEFINE(call_trace);
BOOST_SPIRIT_DEFINE(call_trace_decl);
BOOST_SPIRIT_DEFINE(call_trace_list);


x3::rule<class id_list, std::vector<std::string>> id_list;

auto const id_list_def = (id % ',') | eps;

x3::rule<class stub, data::stub> const stub;
auto const stub_def =
			lexeme["stub" >> skipper]
		>> 	function_name
		>> "=>"
		>>	("void" | id)
		>> '(' >> ((id % ',') | eps)
		>> ')' >> ';';



BOOST_SPIRIT_DEFINE(stub);
BOOST_SPIRIT_DEFINE(id_list);

x3::rule<class fake, data::fake> const fake;
auto const fake_def =
			lexeme["fake" >> skipper]
		>>  function_name
		>>  "=>"
		>>  code_section;

BOOST_SPIRIT_DEFINE(fake);


}
}
}



#endif /* MW_TEST_PARSER_FUNCTIONAL_HPP_ */
