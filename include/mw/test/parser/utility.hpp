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
#include <mw/test/parser/config.hpp>

namespace mw
{
namespace test
{
namespace parser
{

auto append = [](auto &ctx)
		{
			_val(ctx) += _attr(ctx);
		};

x3::rule<class quoted_string, std::string> const quoted_string;

auto const quoted_string_def =
		lexeme['"' >> *(!lit('"') >> char_) >> '"'];


x3::rule<class squoted_string, std::string> const squoted_string;

auto const squoted_string_def =
		lexeme["'" >> *(!lit("'") >> char_) >> "'"];


auto qappend = [](auto &ctx)
		{
			_val(ctx) += '"';
			_val(ctx) += _attr(ctx);
			_val(ctx) += '"';
		};

auto sqappend = [](auto &ctx)
		{
			_val(ctx) += "'";
			_val(ctx) += _attr(ctx);
			_val(ctx) += "'";
		};

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
