/**
 * @file   parse_mwt.hpp
 * @date   31.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_MWT_FILE_HPP_
#define MW_TEST_MWT_FILE_HPP_

#include <string>
#include <boost/filesystem.hpp>
#include <mw/test/ast/main.hpp>
#include <mw/test/data/main.hpp>
#include <mw/test/data/location.hpp>
#include <mw/test/error_log.hpp>

#include <exception>
#include <future>

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


struct mwt_file : ast::main
{

	using iterator = ast::code::iterator;

	boost::filesystem::path file_name;
	std::string buffer;


	iterator begin;
	iterator end;

	std::size_t get_line	  	(iterator itr) const {return boost::spirit::get_line(itr);}
	iterator	get_line_start	(iterator itr) const {return boost::spirit::get_line_start(begin, itr);}
	boost::iterator_range<iterator>
				get_current_line(iterator itr) const {return boost::spirit::get_current_line(begin, itr, end);}
	std::size_t get_column		(iterator itr) const {return boost::spirit::get_column(begin, itr);}

	data::location get_location (iterator itr) const
	{
		return	{
					file_name.string(),
					get_line(itr),
					get_column(itr)
				};
	};

	ast::main 	parse(const boost::filesystem::path & path);

};

inline std::shared_ptr<mwt_file> parse_single_mwt(boost::filesystem::path p)
{
	auto pp = std::make_shared<mwt_file>();
	note(p.string()) << "Started parsing..." << std::endl;

	pp->parse(p);
	return pp;
};

std::vector<std::shared_ptr<mwt_file>> parse_file_set(const std::vector<boost::filesystem::path> & p,
													  const std::vector<boost::filesystem::path>& include_path = {},
													  std::launch async_mode = std::launch::deferred);


data::main to_data(	const std::vector<std::shared_ptr<mwt_file>> & parsed,
					const std::vector<boost::filesystem::path>& include_path = {},
					std::launch async_mode = std::launch::deferred);






}
}



#endif /* MW_TEST_MWT_FILE_HPP_ */
