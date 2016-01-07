/**
 * @file   code.hpp
 * @date   14.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_CODE_HPP_
#define MW_TEST_PARSER_CODE_HPP_

#include <mw/test/parser/config.hpp>
#include <mw/test/parser/utility.hpp>
#include <mw/test/parser/comment.hpp>
#include <mw/test/parser/id.hpp>
#include <mw/test/ast/code.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::code_list,
	(mw::test::ast::code::iterator, _begin)
	(std::vector<std::string>, 		  data)
	(mw::test::ast::code::iterator, _end)
);

BOOST_FUSION_ADAPT_STRUCT(
	mw::test::ast::code,
	(mw::test::ast::code::iterator, _begin)
	(mw::test::ast::code::iterator, _end)
);

namespace mw
{
namespace test
{
namespace parser
{

enum ops
{
	equals,
	not_equal,
	greater_equal,
	lesser_equal,
	greater,
	lesser,
	tolerance,
	assign,
	pointer,
	shift_r,
	shift_l,
	shift_assign_r,
	shift_assign_l,
	modulo,
	modulo_assign,
};

struct operators_t : x3::symbols<ops>
{
	operators_t()
    {
        add ("="   , assign		  )
        	("!="  , not_equal	  )
            ("=="  , equals       )
            (">="  , greater_equal)
            ("<="  , lesser_equal )
            (">"   , greater      )
            ("<"   , lesser       )
            ("+/-" , tolerance    )
			("->"  , pointer	  )
			("%"   , modulo)
			("%="  , modulo_assign)
        ;
    }

} operators;

x3::rule<class pointy_ops, std::string> const pointy_ops;

auto const pointy_ops_def
	= string("->")
	| string(">>")
	| string("<<")
	| string(">>=")
	| string(">>=")
	| string("<")
	| string(">")
	| string("<=")
	| string(">=")
	| string("=")
	;

x3::rule<class square_par_code_chunk, std::string> const square_par_code_chunk;
x3::rule<class round_par_code_chunk , std::string> const round_par_code_chunk ;
x3::rule<class curly_par_code_chunk , std::string> const curly_par_code_chunk ;
x3::rule<class pointy_par_code_chunk, std::string> const pointy_par_code_chunk ;

x3::rule<class code_list, 			ast::code_list > const code_list;
x3::rule<class code_list_step,		std::string > const code_list_step;

x3::rule<class code_chunk, 					ast::code > const code_chunk;
x3::rule<class code_chunk_no_ops, 			ast::code > const code_chunk_no_ops;

x3::rule<class code_chunk_in, 				std::string> const code_chunk_in;
x3::rule<class code_chunk_in_step, 			std::string> const code_chunk_step;
x3::rule<class code_chunk_in_step_no_ops, 	std::string> const code_chunk_step_no_ops;

x3::rule<class code_section,  ast::code>	 		const code_section;
x3::rule<class code_function, ast::code>			const code_function;
x3::rule<class code_location, ast::code::iterator>  const code_location;


auto const square_par_code_chunk_def =
		char_("[") >> *(!lit("]") >> code_chunk_step) >> char_("]")
		;

auto const round_par_code_chunk_def =
		char_("(") >> *(!lit(")") >> code_chunk_step) >> char_(")")
		;

auto const curly_par_code_chunk_def =
		char_("{") >> *(!lit("}") >> code_chunk_step) >> char_("}")
		;

auto const pointy_par_code_chunk_def =
		char_("<") >> *(!lit(">") >> code_chunk_step) >> char_(">")
		;


auto const code_chunk_step_def =
		  ((!char_("[]{}()<>") >> char_)
		   | square_par_code_chunk
		   | round_par_code_chunk
		   | curly_par_code_chunk
		   | pointy_par_code_chunk
		   | quoted_string
		   | squoted_string
		   | pointy_ops);

auto const code_chunk_step_no_ops_def =
		  ((!char_("[]{}()<>") >> char_)
		   | square_par_code_chunk
		   | round_par_code_chunk
		   | curly_par_code_chunk
		   | pointy_par_code_chunk
		   | quoted_string
		   | squoted_string);



namespace code
{
auto set_beg = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx)._begin = itr;
		};

auto set_end = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx)._end = itr;
		};

}

auto const code_function_def =
			eps[code::set_beg] >>
			+(!lit('(') >> code_chunk_step ) >>
			'(' >> code_list >> ')' >>
			'{' >> *(!lit('}') >> code_chunk_step) >> '}'
			>>
			eps[code::set_end] >> -omit[';']
		;

auto const code_chunk_def =
		eps[code::set_beg] >>
			omit[ lexeme[*(!lit(';') >> code_chunk_step)]]
			  >> eps[code::set_end] >> omit[';'];
		;

auto const code_section_def =
			 lexeme[
				'{' >>
				eps[code::set_beg] >>
				omit[*(!lit('}') >> code_chunk_step)]
				>> eps[code::set_end]
				>> '}'
			 ] >> -lit(';');
		;

auto const code_list_step_def =
		lexeme[*(!lit(',') >> omit[*(space | eol)] >> code_chunk_step_no_ops)];

auto const code_list_def =
				code_location
			>> (code_list_step % ',')
			>> 	code_location;


auto const code_location_def = eps[l([](auto &ctx)
		{
			ast::code::iterator it = x3::_where(ctx).begin();
			_val(ctx) = it;
		})];


BOOST_SPIRIT_DEFINE(square_par_code_chunk);
BOOST_SPIRIT_DEFINE(round_par_code_chunk );
BOOST_SPIRIT_DEFINE(curly_par_code_chunk );
BOOST_SPIRIT_DEFINE(pointy_par_code_chunk);
BOOST_SPIRIT_DEFINE(code_chunk_step);
BOOST_SPIRIT_DEFINE(code_chunk);
BOOST_SPIRIT_DEFINE(code_list);
BOOST_SPIRIT_DEFINE(code_list_step);
BOOST_SPIRIT_DEFINE(pointy_ops);
BOOST_SPIRIT_DEFINE(code_location);
BOOST_SPIRIT_DEFINE(code_section);
BOOST_SPIRIT_DEFINE(code_chunk_step_no_ops);
BOOST_SPIRIT_DEFINE(code_function);

}
}
}

#endif /* MW_TEST_PARSER_CODE_HPP_ */
