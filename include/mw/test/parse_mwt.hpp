/**
 * @file   parse_mwt.hpp
 * @date   31.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSE_MWT_HPP_
#define MW_TEST_PARSE_MWT_HPP_

#include <string>
#include <boost/filesystem.hpp>
#include <mw/test/ast/main.hpp>
#include <mw/test/data/main.hpp>
#include <mw/test/data/location.hpp>
#include <exception>

namespace mw
{
namespace test
{

struct incomplete_parse : std::runtime_error
{
	using std::runtime_error::runtime_error;
};

struct multiple_definition : std::runtime_error
{
	using std::runtime_error::runtime_error;
};

struct object_not_found : std::runtime_error
{
	using std::runtime_error::runtime_error;
};

struct too_few_tpl_args : std::runtime_error
{
	using std::runtime_error::runtime_error;
};


struct mwt_parser
{

	using iterator = ast::code::iterator;

	boost::filesystem::path file_name;
	std::string buffer;


	iterator begin;
	iterator end;

	std::size_t get_line	  	(iterator itr) {return boost::spirit::get_line(itr);}
	iterator	get_line_start	(iterator itr) {return boost::spirit::get_line_start(begin, itr);}
	boost::iterator_range<iterator>
				get_current_line(iterator itr) {return boost::spirit::get_current_line(begin, itr, end);}
	std::size_t get_column		(iterator itr) {return boost::spirit::get_column(begin, itr);}

	data::location get_location (iterator itr)
	{
		return	{
					file_name.string(),
					get_line(itr),
					get_column(itr)
				};
	};

	mwt_parser(const iterator & begin, const iterator & end) : begin(begin), end(end) {};


	ast::main parse();
	data::main transform(const ast::main & m);
	void load_file();


};




}
}



#endif /* MW_TEST_PARSE_MWT_HPP_ */
