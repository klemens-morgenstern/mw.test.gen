/**
 * @file   error_log.hpp
 * @date   17.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_ERROR_LOG_HPP_
#define MW_TEST_ERROR_LOG_HPP_

#include <iostream>

namespace mw
{
namespace test
{

void   set_error();
void reset_error();
bool   get_error();

void set_warn_as_error(bool b);
bool 	 warn_as_error();


inline std::ostream& error(const std::string &file_name, std::size_t line_nr = 0)
{
	set_error();
	return std::cerr <<  file_name << "(" << line_nr << ") Error: ";
}

inline std::ostream& warn(const std::string &file_name, std::size_t line_nr = 0)
{
	if (warn_as_error())
		set_error();
	return std::cerr <<  file_name << "(" << line_nr << ") Warn: ";
}

inline std::ostream& note(const std::string &file_name, std::size_t line_nr = 0)
{
	return std::cerr <<  file_name << "(" << line_nr << ") Note: ";
}


inline std::ostream& internal_error(const std::string &file_name, std::size_t line_nr = 0)
{
	set_error();
	return std::cerr <<  file_name << "(" << line_nr << ") Internal Error: ";
}

inline std::ostream& internal_warn(const std::string &file_name, std::size_t line_nr = 0)
{
	return std::cerr <<  file_name << "(" << line_nr << ") Internal Warn: ";
}

inline std::ostream& internal_note(const std::string &file_name, std::size_t line_nr = 0)
{
	return std::cerr <<  file_name << "(" << line_nr << ") Internal Note: ";
}


}
}

#define INTERNAL_ERROR() ::DoxyFrame::Log::InternalError(__FILE__, __LINE)
#define INTERNAL_WARN()  ::DoxyFrame::Log::InternalWarn (__FILE__, __LINE)
#define INTERNAL_NOTE()  ::DoxyFrame::Log::InternalNote (__FILE__, __LINE)

#endif /* ERROR_LOG_HPP_ */
