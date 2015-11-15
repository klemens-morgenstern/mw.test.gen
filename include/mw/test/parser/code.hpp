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

namespace mw
{
namespace test
{
namespace parser
{

enum ops
{
	equals,
	greater_equal,
	lesser_equal,
	greater,
	lesser,
	tolerance,
};

struct operators_ : x3::symbols<ops>
{
	operators_()
    {
        add
            ("=="  , equals       )
            (">="  , greater_equal)
            ("<="  , lesser_equal )
            (">"   , greater      )
            ("<"   , lesser       )
            ("+/-" , tolerance    )
        ;
    }

} operators;


x3::rule<class square_par_code_chunk> const square_par_code_chunk("square_par_code_chunk");
x3::rule<class round_par_code_chunk>  const round_par_code_chunk ("round_par_code_chunk");
x3::rule<class curly_par_code_chunk>  const curly_par_code_chunk ("curly_par_code_chunk");
x3::rule<class quote_par_code> 		  const quote_par_code 		 ("quote_par_code");

x3::rule<class unaligned_code_chunk> const unaligned_code_chunk("unaligned_code_chunk");


auto const square_par_code_chunk_def =
		"[" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> "]"
		;

auto const round_par_code_chunk_def =
		"(" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> ")"
		;

auto const curly_par_code_chunk_def =
		"{" >> *(!(char_("[]{}()") | eol) >> *no_skip[!(eol | space)]) >> "}"
		;

auto const quote_par_code_def =
		('"' >> *(!char_('"') >>  ("\\\"" | char_ ) ) >> '"')
		| ('\'' >> char_ >> '\'')
		;

auto const unaligned_code_chunk_def =
		  +(operators
		| square_par_code_chunk_def
		| round_par_code_chunk_def
		| curly_par_code_chunk_def
		| quote_par_code_def
		| no_skip[+(!(char_(";")) >> char_)]);





x3::rule<class aligned_code_chunk> const bracket_aligned_code_chunk("bracket_aligned_code_chunk");

auto const bracket_aligned_code_chunk_def =
		no_skip["[" >>
				!(char_("[]")
				>> ( bracket_aligned_code_chunk | *char_))
				>> "]"]
		;


x3::rule<class aligned_code_chunk> const aligned_code_chunk("aligned_code_chunk");

auto const aligned_code_chunk_def =
		no_skip["[" >>
				!(char_("[]")
				>> (bracket_aligned_code_chunk | *char_))
				>> "]"]
		;

///a code_chunk fragment.
x3::rule<class code_chunk> const code_chunk("code_chunk");

auto const code_chunk_def = aligned_code_chunk | unaligned_code_chunk;


x3::rule<class code> const code("code");

auto const code_def = ((+aligned_code_chunk) | unaligned_code_chunk) >> ";";


BOOST_SPIRIT_DEFINE(square_par_code_chunk);
BOOST_SPIRIT_DEFINE(round_par_code_chunk);
BOOST_SPIRIT_DEFINE(curly_par_code_chunk);
BOOST_SPIRIT_DEFINE(unaligned_code_chunk);
BOOST_SPIRIT_DEFINE(bracket_aligned_code_chunk);
BOOST_SPIRIT_DEFINE(aligned_code_chunk);

BOOST_SPIRIT_DEFINE(code_chunk);
BOOST_SPIRIT_DEFINE(code);


}
}
}

#endif /* MW_TEST_PARSER_CODE_HPP_ */
