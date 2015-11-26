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
#include <mw/test/data/code.hpp>


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

struct operators_ : x3::symbols<ops>
{
	operators_()
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
	;

x3::rule<class square_par_code_chunk, std::string> const square_par_code_chunk;
x3::rule<class round_par_code_chunk , std::string> const round_par_code_chunk ;
x3::rule<class curly_par_code_chunk , std::string> const curly_par_code_chunk ;
x3::rule<class pointy_par_code_chunk, std::string> const pointy_par_code_chunk ;

x3::rule<class code_chunk, 			data::code > const code_chunk;
x3::rule<class code_chunk_in, 		std::string> const code_chunk_in;
x3::rule<class code_chunk_in_step, 	std::string> const code_chunk_step;


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
		   | quoted_string
		   | squoted_string
		   | pointy_ops);


namespace code
{
auto set_beg = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx)._begin = itr;
		};

auto set_end = [](auto &ctx)
		{
			using iterator = boost::spirit::line_pos_iterator<typename std::string::iterator>;
			iterator itr = x3::_where(ctx).begin();
			_val(ctx)._end = itr;
		};



}

auto const code_chunk_def =
		eps[code::set_beg] >>
		omit[
			 lexeme[*(!lit(';') >> code_chunk_step)] >> char_(';')
			 ] >> eps[code::set_end];
		;


BOOST_SPIRIT_DEFINE(square_par_code_chunk);
BOOST_SPIRIT_DEFINE(round_par_code_chunk );
BOOST_SPIRIT_DEFINE(curly_par_code_chunk );
BOOST_SPIRIT_DEFINE(pointy_par_code_chunk);
BOOST_SPIRIT_DEFINE(code_chunk_step);
BOOST_SPIRIT_DEFINE(code_chunk);
BOOST_SPIRIT_DEFINE(pointy_ops);

}
}
}

#endif /* MW_TEST_PARSER_CODE_HPP_ */
