/**
 * @file   utility.hpp
 * @date   15.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_UTILITY_HPP_
#define MW_TEST_PARSER_UTILITY_HPP_


#include <string>
#include <mw/test/parsers/config.hpp>

namespace mw
{
namespace test
{
namespace parser
{
///General appending tool for parsers.
auto append = [](auto &ctx)
		{
			_val(ctx) += _attr(ctx);
		};

///A quoted string
/** @return std::string
 * @code{.enbf}
 * <quoted string> ::= '"' >> (!'"')* >> '"' ;
 * @endcode
 */
x3::rule<class quoted_string, std::string> const quoted_string;

auto const quoted_string_def =
		lexeme['"' >> *(
				  !lit('"') >> char_)  >> '"'];

///A single quoted string
/** @return std::string
 * @code{.enbf}
 * <single quoted string> ::= "'" >> (!"'")* >> "'" ;
 * @endcode
 * @remark Multibyte chars are valid C++
 */
x3::rule<class squoted_string, std::string> const squoted_string;

auto const squoted_string_def =
		lexeme["'" >> *(
				  !lit('\'') >> char_) >> "'"];

///Append a string with quotes
auto qappend = [](auto &ctx)
		{
			_val(ctx) += '"';
			_val(ctx) += _attr(ctx);
			_val(ctx) += '"';
		};

///Append a string with single quotes
auto sqappend = [](auto &ctx)
		{
			_val(ctx) += "'";
			_val(ctx) += _attr(ctx);
			_val(ctx) += "'";
		};

///Log rule, parses a string, which may containt (single) quoted strings
/**@code{.enbf}
 * <log> ::= 'log' char* ';' ;
 * @endcode
 */
x3::rule<class log, std::string> const log;

auto const log_def =
		lexeme["log" >> (eol | space)
			   	>> *(!lit(";")
				>> ((!char_(";\"'") >> char_[append]) |
			        quoted_string[qappend] |
					squoted_string[sqappend]))
			] >> ";";

BOOST_SPIRIT_DEFINE(quoted_string, squoted_string, log);

}
}
}



#endif /* MW_TEST_PARSER_UTILITY_HPP_ */
