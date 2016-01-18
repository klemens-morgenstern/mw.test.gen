/**
 * @file   parse_mwt.cpp
 * @date   31.12.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/fstream.hpp>
#include <mw/test/mwt_file.hpp>
#include <mw/test/parser/main.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

namespace mw
{
namespace test
{

inline std::size_t needed_template_args(const ast::test_object & to)
{
	std::size_t res = 0;
	for (auto & x : to.tpl_args)
	{
		if (x.def_arg.empty())
			res ++;
	}
	return res;
}

inline bool is_template(const ast::test_object& ato)
{
	return !ato.tpl_args.empty();
}

inline std::string make_template_id(const ast::test_object& to, const ast::obj_id &id)
{
	std::stringstream ss;

	std::vector<std::string> buf;
	buf.resize(to.tpl_args.size());

	std::transform(to.tpl_args.begin(), to.tpl_args.end(), buf.begin(), [](const ast::tpl_arg &ta){return ta.def_arg;});
	std::copy(id.tpl_args.begin(), id.tpl_args.end(), buf.begin());

	ss << id.name << "<";

	for (auto & elem : buf)
	{
		if (&elem  != &buf.front())
			ss << ", ";
	}

	ss << ">";
	return ss.str();
}

inline std::string load_file(const boost::filesystem::path &p)
{
	stringstream ss;
	ss  << boost::filesystem::ifstream(p).rdbuf();
	return ss.str();
}

ast::main mwt_file::parse(const boost::filesystem::path &path)
{
	namespace x3 = boost::spirit::x3;

	buffer = load_file(path);


	using iterator = ast::code::iterator;

	begin = iterator(buffer.begin());
	end   = iterator(buffer.end());

	iterator itr{buffer.begin()};
	iterator end{buffer.end  ()};

	ast::main &mn = *static_cast<ast::main* const>(this);

	x3::phrase_parse(itr, end, parser::main, parser::skipper, mn);

	if (itr != end)
		throw incomplete_parse("Error"); //itr, end

	return mn;
}



struct transformer
{
	const mwt_file & file;

	data::main & mn;

	data::group 		operator()(const ast::group & grp);
	data::use_file 		operator()(const ast::use_file & grp);
	std::shared_ptr<data::test_object>
						operator()(const ast::test_object & grp);
	data::doc_t			operator()(const ast::doc_t & doc);
	std::weak_ptr<data::object>
						operator()(const ast::obj_id &id);
	data::use_file_type	operator()(ast::use_file_type uft);

};

std::weak_ptr<data::object> transformer::operator()(const ast::obj_id &id)
{
	std::shared_ptr<ast::test_object> to;

/*	for (auto & t : mn.test_objects)
	{
		auto res = std::find_if(mn.test_objects.begin(), mn.test_objects.end(),
				[&](const data::object_p & op){return op->id == id.name;});

		if (res == mn.test_objects.end())
			throw object_not_found(id.name);

		//to = *res;
	}*/

	if (needed_template_args(*to) > id.tpl_args.size())
		throw too_few_tpl_args("");


}


data::use_file_type	transformer::operator()(ast::use_file_type uft)
{
	switch(uft)
	{
	case ast::tests_file:	return data::use_file_type::tests_file;
	case ast::file:			return data::use_file_type::file;
	case ast::include:		return data::use_file_type::include;
	default:
		return static_cast<data::use_file_type>(uft);
	}
}

data::doc_t			transformer::operator()(const ast::doc_t & doc)
{
	data::doc_t out;

	out.body = doc.body;
	out.head = doc.head;

	return out;
}


data::use_file 		transformer::operator()(const ast::use_file & grp)
{
	data::use_file out;

	out.doc 		= (*this)(grp.doc);
	out.location	= file.get_location(grp.location);
	out.filename	= grp.filename;
	out.type		= (*this)(grp.type);

	return out;
}

std::shared_ptr<data::test_object> 	transformer::operator()(const ast::test_object & in)
{
	std::shared_ptr<data::test_object> to;

	to->doc = (*this)(in.doc);
	//to->id  = in.id.
	return to;
}

data::group transformer::operator()(const ast::group & grp)
{
	data::group out;

	out.content.	 resize(grp.content.	size());

	std::transform(grp.content.begin(), 	grp.content.end(), 	out.content.begin(), *this);

	out.name = grp.name;
	out.doc	 = (*this)(grp.doc);
	out.loc	 = file.get_location(grp.location);
	return out;
}

/*
data::main mwt_file::transform() const
{
	data::main out;

	out.use_files.	 resize(use_files.	size());
	out.test_objects.resize(test_objects.size());
	out.groups.		 resize(groups.		size());


	std::transform(use_files.begin(), 		use_files.end(), 	out.use_files.begin(), 		transformer{*this, out});
	std::transform(test_objects.begin(), 	test_objects.end(),	out.test_objects.begin(), 	transformer{*this, out});
	std::transform(groups.begin(), 			groups.end(), 		out.groups.begin(), 		transformer{*this, out});

	return out;
}*/

std::vector<std::shared_ptr<mwt_file>> parse_file_set(const std::vector<boost::filesystem::path> & p,
													  const std::vector<boost::filesystem::path>& include_path,
													  std::launch async_mode)
{
	using namespace std;
	namespace fs = boost::filesystem;
	using future = std::future<std::shared_ptr<mwt_file>>;

	vector<future> futures;

	vector<fs::path> already_parsing; //values alreading in parsing/parsed

	auto get_path = [&include_path](const boost::filesystem::path & p) -> fs::path
	{
		for (auto & ip : include_path)
		{
			auto pth = ip / p;
			if (fs::is_regular_file(pth)) //found the file.
				return pth;
		}
		error(__FILE__, __LINE__) << "File " << p.string() << " not found." <<  endl;
		exit(1);
		return p; //for compiler warning.
	};


	//ok, construct the async stuff.
	for (auto & in : p)
	{
		auto gp = fs::absolute(get_path(in));
		already_parsing.push_back(gp);
		futures.push_back(async(async_mode, parse_single_mwt, gp));
	}


	while (!futures.empty())//ok, always takeout the first and create new futures from the includes.
	{
		auto res = futures.front().get();//ok, get the first.
		futures.erase(futures.begin()); //delete the currently taken.

		for (auto & f : res->use_files)
		{
			if (f.type != ast::include) //ok, not an mwt file
				continue;
			auto inc = fs::path(f.filename);
			if (inc.extension() != ".mwt") //no mwt file, no need to parse.
				continue;

			auto gp = fs::absolute(get_path(inc)); //ok, i got a path.

			//Now: is this file already parsed?

			auto cnt = std::count(already_parsing.begin(), already_parsing.end(), gp);

			if (cnt > 0) //already parsed
				continue;

			already_parsing.push_back(gp);
			futures.push_back(async(async_mode, parse_single_mwt, gp));
		}

	}


	vector<shared_ptr<mwt_file>> ret;


	return ret;

}


}
}
