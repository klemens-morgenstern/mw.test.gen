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
#include <mutex>
#include <mw/test/data/location.hpp>

namespace mw
{
namespace test
{

void   set_error();
void reset_error();
bool   get_error();

void set_warn_as_error(bool b);
bool 	 warn_as_error();


inline void print_error_location(const std::string & line, size_t column)
{
	using namespace std;
	cerr << line << endl;
	cerr << std::string(column-1, ' ') << '^' << endl;
}

inline void print_error_location(const std::string & line, size_t column, size_t range)
{
	using namespace std;
	cerr << line << endl;
	cerr << std::string(column-1, ' ') << std::string(range, '^') << endl;
}

inline std::ostream& error(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;
	set_error();
	return cerr <<  file_name << "(" << line_nr << ") Error: ";
}

inline std::ostream& error(const std::string &file_name, size_t line_nr, size_t column)
{
	using namespace std;
	set_error();
	return cerr <<  file_name << "(" << line_nr << "," << column << ") Error: ";
}



inline std::ostream& warn(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;
	if (warn_as_error())
		set_error();
	return cerr <<  file_name << "(" << line_nr << ") Warn: ";
}

inline std::ostream& warn(const std::string &file_name, size_t line_nr, size_t column)
{
	using namespace std;
	if (warn_as_error())
		set_error();
	return cerr <<  file_name << "(" << line_nr << "," << column << ") Warn: ";
}

inline std::ostream& note(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;

	return cerr <<  file_name << "(" << line_nr << ") Note: ";
}

inline std::ostream& note(const std::string &file_name, size_t line_nr, size_t column)
{
	using namespace std;

	return cerr <<  file_name << "(" << line_nr << "," << column << ") Note: ";
}

inline std::ostream& internal_error(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;
	set_error();
	return cerr <<  file_name << "(" << line_nr << ") Internal Error: ";
}

inline std::ostream& internal_warn(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;
	return cerr <<  file_name << "(" << line_nr << ") Internal Warn: ";
}

inline std::ostream& internal_note(const std::string &file_name, size_t line_nr = 0)
{
	using namespace std;
	return cerr <<  file_name << "(" << line_nr << ") Internal Note: ";
}

inline std::ostream& error(const data::location & loc) { return error(loc.file_name, loc.line); }
inline std::ostream& warn(const data::location & loc)  { return warn(loc.file_name, loc.line); }
inline std::ostream& note(const data::location & loc)  { return note(loc.file_name, loc.line); }
inline void print_error_location(const data::location & loc) {print_error_location(loc.file_name, loc.column);}



}
}

#define INTERNAL_ERROR() ::DoxyFrame::Log::InternalError(__FILE__, __LINE)
#define INTERNAL_WARN()  ::DoxyFrame::Log::InternalWarn (__FILE__, __LINE)
#define INTERNAL_NOTE()  ::DoxyFrame::Log::InternalNote (__FILE__, __LINE)

#endif /* ERROR_LOG_HPP_ */
