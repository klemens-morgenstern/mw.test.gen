/**
 * @file   code.hpp
 * @date   22.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_OPERATIONS_HPP_
#define MW_TEST_DATA_OPERATIONS_HPP_

#include <vector>
#include <string>
#include <mw/test/data/code.hpp>
#include <boost/variant.hpp>

namespace mw
{
namespace test
{
namespace data
{

enum level_t
{
	assertion,
	expectation,
};

struct check_qualification
{
	bool static_   = false;
	bool bitwise  = false;
	bool ranged    = false;
	bool critical = false;
};

//location is inside the code
struct code_check
{
	doc doc;
	code data;
	level_t lvl;
	check_qualification qual;
};

struct execute_check
{
	doc doc;
	code::iterator location;
	level_t lvl;
	bool critical;
};

struct no_execute_check
{
	doc doc;
	code::iterator location;
	level_t lvl;
	bool critical;
};



struct throw_check;
struct no_throw_check;
struct any_throw_check;
struct critical_section;

using check_entry =
		boost::variant<
			execute_check,
			no_execute_check,
			code_check,
			throw_check,
			no_throw_check,
			any_throw_check,
			critical_section,
			code
		>;


struct throw_check
{
	doc doc;
	bool critical = false;
	void set_critical() {critical = true;}

	code::iterator _begin;
	code::iterator _end;

	code::iterator begin() const {return _begin;};
	code::iterator end()   const {return _end;}

	std::vector<check_entry> steps;
	code_list exceptions;
	level_t lvl;


};

struct no_throw_check
{
	doc doc;
	bool critical = false;
	void set_critical() {critical = true;}

	code::iterator _begin;
	code::iterator _end;

	code::iterator begin() const {return _begin;};
	code::iterator end()   const {return _end;}

	std::vector<check_entry> steps;
	level_t lvl;
};

struct any_throw_check
{
	doc doc;
	bool critical = false;
	void set_critical() {critical = true;}

	code::iterator _begin;
	code::iterator _end;

	code::iterator begin() const {return _begin;};
	code::iterator end()   const {return _end;}

	std::vector<check_entry> steps;
	level_t lvl;

};



struct critical_section
{
	doc doc;
	code::iterator _begin;
	code::iterator _end;
	std::vector<check_entry> steps;

	code::iterator begin(){return _begin;}
	code::iterator end()  {return _end;};

};



}
}
}
/*



 */

#endif /* MW_TEST_DATA_OPERATIONS_HPP_ */
