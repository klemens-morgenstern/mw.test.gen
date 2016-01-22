/**
 * @file   template_instantiate.cpp
 * @date   19.01.2016
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <mw/test/template_instantiate.hpp>

#include <algorithm>
#include <iostream>
#include <cassert>
#include <mw/test/error_log.hpp>

using namespace std;

namespace mw {
namespace test {

/**
 * This is the state machine, by which
 * @dot
digraph G
{
  S0[shape=doublecircle];
  node[shape=circle];
  S0->S0[label="[^\"_A-Za-z]"];

  S0->S1[label="[\"]"];
  S1->S0[label="[\"]"];
  S1->S1[label="[^\\\"]"];
  S1->S2[label="[\\]"];
  S2->S1[label="."];

  S0->S3[label="[']"];
  S3->S0[label="[']"];
  S3->S3[label="[^\\']"];
  S3->S4[label="[\\]"];
  S4->S3[label="."];

  S0->S5[label="[_A-Za-z]"];
  S5->S5[label="[_A-Za-z0-9]"];
  S5->S0[label="[^_A-Za-z0-9]"];

  S0->S6[label="[/]"];
  S6->S0[label="[^/*A-Za-z_]"];
  S6->S7[label="[/]"];
  S7->S0[label="[\\n\\r]"];

  S7->S7[label="[^\\n\\r]"];

  S6->S8[label="[*]"];
  S8->S8[label="[^*]"];
  S8->S9[label="[*]"];
  S9->S8[label="[^\/]"];

  S6->S5[label="[_A-Za-z]"];

  S9->S9[label="[*]"];
  S9->S0[label="[\/]"];
}

| Edge   | Action |
|--------|--------|
| S0->S5 | Set Token Iterator |
| S6->S5 | Set Token Iterator |
| S5->S0 | Push Id |

 */


string mw::test::template_instantiate::replace(const string& data, const vector<replacement>& replacements)
{

	enum states {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9};
	string buf;
	states state = S0;


	//begin of the ucrrent view
	auto curr_beg = data.begin();
	auto id_beg = data.begin();

	auto itr = data.begin();


	//pushs the current state into the buffer
	auto start_id = [&]{
		id_beg = itr;
	};

	auto push	 = [&]{
		buf.append( curr_beg, itr);
	};

	auto push_id = [&]{
		//get the current id
		string id(id_beg, itr);
		//now check if it must be replaced
		auto it =  std::find_if(replacements.begin(), replacements.end(),
					[&id](const replacement & rpl){ return rpl.id == id;});

		//if i found a replacement, apply it, elsewise just move on.
		if (it != replacements.end())
		{

			buf.append(curr_beg, id_beg);
			buf.append(it->replace.begin(), it->replace.end());

			curr_beg = itr;
		}
	};

	//Matches: [_A-Za-z]
	auto is_id_beg = [](char c)
			{
				return (c == '_')
					|| ((c >= 'A') && (c <= 'Z'))
					|| ((c >= 'a') && (c <= 'z'));
			};
	//Matches: [_A-Za-z0-9]
	auto is_id_char = [](char c)
			{
				return (c == '_')
					|| ((c >= 'A') && (c <= 'Z'))
					|| ((c >= 'a') && (c <= 'z'))
					|| ((c >= '0') && (c <= '9'));
			};

	while (itr != data.end())
	{
		char c = *itr;
		switch (state)
		{
		case S0:
		{
			if (c == '"')
				state = S1;
			else if (c == '\'')
				state = S3;
			else if (is_id_beg(c))
			{
				//must be part of an id
				state = S5;
				start_id();
			}
			else if (c == '/')
			{
				state = S6;
			}
			//if none of occures, we just stay in this state.
		}
		break;
		case S1:
		{
			if (c == '"')
				state = S0;
			else if (c == '\\')
				state = S2;
		}
		break;
		case S2:
			state = S1;
		break;
		case S3:
		{
			if (c == '\'')
				state = S0;
			else if (c == '\\')
				state = S4;
		}
		break;
		case S4:
		{
			state = S1;
		}
		break;
		case S5:
		{
			if (!is_id_char(c))
			{
				state = S0;
				push_id();
			}
		}
		break;
		case S6:
		{
			if (is_id_beg(c))
			{
				//start the id
				state = S5;
				start_id();
			}
			else if (c == '/')
				state = S7;
			else if (c == '*')
				state = S8;
			else
				state = S0;
		}
		break;
		case S7:
		{
			if ((c == '\n') || (c == '\r'))
				state = S0;
		}
		break;
		case S8:
		{
			if (c == '*')
				state = S9;

		}
		break;
		case S9:
		{
			if (c == '/')
				state = S0;
			else if (c != '*')
				state = S8;
		}
		break;
		}
		itr++;

	}

	if (state == S5)//in id
		push_id();
	else
		push(); //just push the chars onto the outstring

	return buf;
}

data::object_tpl_p template_instantiate::select_type(const ast::test_object & to)
{
	switch (to.type)
	{
	case ast::object:
	{
		auto p = std::make_shared<data::test_object_tpl>();
		if (to.inheritance.size())
		{

		}

	}

	case ast::classification:   return std::make_shared<data::test_classification_tpl>();
	case ast::composition:      return std::make_shared<data::test_composition_tpl>();
	case ast::class_:           return std::make_shared<data::test_class_tpl>();
	case ast::case_:            return std::make_shared<data::test_case_tpl>();
	case ast::step:             return std::make_shared<data::test_step_tpl>();
	case ast::sequence:         return std::make_shared<data::test_sequence_tpl>();
	default:
		assert(false);

	}
	return nullptr;
}

data::object_tpl_p template_instantiate::make(const ast::test_object & to)
{
	assert(to.is_template());//check internal error


	auto p = select_type(to);


	return p;
}



} /* namespace test */
} /* namespace mw */


