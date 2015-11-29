/**
 * @file   code.hpp
 * @date   26.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_CODE_HPP_
#define MW_TEST_DATA_CODE_HPP_

#include <string>
#include <iterator>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace mw
{
namespace test
{
namespace data
{

struct code
{
	typedef
			boost::spirit::line_pos_iterator<
			BOOST_DEDUCED_TYPENAME
			std::string::iterator>
			iterator;

	iterator _begin;
	iterator _end;

	iterator begin() const {return _begin;};
	iterator end()   const {return _end;}

	std::string to_string() const
	{
		return std::string(_begin, _end);
	}
	void clear()
	{
		_begin = iterator();
		_end   = iterator();
	}
	std::size_t size()
	{
		return std::distance(_begin, _end);
	}
};

struct code_list
{
	typedef
			boost::spirit::line_pos_iterator<
			BOOST_DEDUCED_TYPENAME
			std::string::iterator>
			iterator;

	iterator _begin;
	iterator _end;

	iterator begin() const {return _begin;};
	iterator end()   const {return _end;}

	std::vector<std::string> data;

	std::string to_string() const
	{
		return std::string(_begin, _end);
	}
	void clear()
	{
		_begin = iterator();
		_end   = iterator();
		data.clear();
	}
	std::size_t size()
	{
		return data.size();
	}
};

}
}
}


#endif /* MW_TEST_DATA_CODE_HPP_ */
