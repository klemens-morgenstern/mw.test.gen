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
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>
#include <mw/test/data/location.hpp>
//#include <mw/test/ast/main.hpp>
//#include <mw/test/data/main.hpp>
//#include <mw/test/data/location.hpp>
//#include <mw/test/error_log.hpp>

#include <exception>
#include <future>

namespace mw
{
namespace test
{

struct parse_failed : std::runtime_error
{
    using std::runtime_error::runtime_error;

};

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

struct mwt_file
{

	using iterator = boost::spirit::line_pos_iterator<typename std::string::const_iterator>;
	boost::filesystem::path file_name;
	std::string buffer;


	iterator _begin = iterator(buffer.cbegin());
	iterator _end   = iterator(buffer.cend());

	iterator end()   const {return _end;}
	iterator begin() const {return _begin;}

	std::size_t get_line	  	(iterator itr) const {return boost::spirit::get_line(itr);}
	iterator	get_line_start	(iterator itr) const {return boost::spirit::get_line_start(_begin, itr);}
	boost::iterator_range<iterator>
				get_current_line(iterator itr) const {return boost::spirit::get_current_line(_begin, itr, _end);}
	std::size_t get_column		(iterator itr) const {return boost::spirit::get_column(_begin, itr);}

	data::location get_location (iterator itr) const
	{
		return	{
					file_name.string(),
					get_line(itr),
					get_column(itr)
				};
	};

	bool is_virtual() const {return file_name.empty(); }

	mwt_file(const boost::filesystem::path & file_name) :
	    file_name(file_name),
	    buffer(
	            static_cast<std::stringstream&>((std::stringstream() << boost::filesystem::fstream(file_name).rdbuf())).str()

	            ) {}
	mwt_file(const std::string & content = "") : buffer(content) {}
	mwt_file(std::string && content) : buffer(std::move(content)) {}
};



}
}



#endif /* MW_TEST_MWT_FILE_HPP_ */
