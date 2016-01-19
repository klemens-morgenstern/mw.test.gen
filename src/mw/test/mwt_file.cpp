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
#include <boost/algorithm/string/trim.hpp>
#include <unordered_map>

#include <mutex>
#include <set>

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

data::use_file_type	transform(ast::use_file_type uft)
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

data::doc_t			transform(const ast::doc_t & doc)
{
	data::doc_t out;

	out.body = doc.body;
	out.head = doc.head;

	return out;
}


std::shared_ptr<data::test_object> 	transform(const ast::test_object & in)
{
	std::shared_ptr<data::test_object> to;

	to->doc =	transform(in.doc);
	//to->id  = in.id.
	return to;
}

data::use_file transform(const ast::use_file & grp, const mwt_file & file)
{
	data::use_file out;

	out.doc 		= transform(grp.doc);
	out.type		= transform(grp.type);

	out.loc			= file.get_location(grp.location);
	out.filename	= grp.filename;

	return out;
};

std::vector<data::use_file> use_file_transform_all(const std::vector<std::shared_ptr<mwt_file>> & parsed)
{
	std::vector<data::use_file> ret;

	for (auto & p : parsed)
	{
		ret.reserve(ret.size() + p->use_files.size());
		for (auto &uf : p->use_files)
			ret.push_back(transform(uf, *p));

	}
	return ret;
};

std::vector<std::string> get_files(const std::vector<std::shared_ptr<mwt_file>> & parsed)
{
	std::vector<std::string> files;

	files.reserve(parsed.size());
	for (auto & p : parsed)
		files.push_back(p->file_name.string());

	return files;
}

std::string transform(const ast::obj_id &id)
{
	/*ok, here's how it works: trim the thing, i..e remove the border cases. This may yield wrong results, since X<2+3> and X<2 + 3> would be two different objects.
	  But this is a corner case, and this tool doesn't perform any semantic C++ tests on purpose */

	std::stringstream res;
	res << boost::trim_copy(id.name);


	if (id.tpl_args.size() > 0)
	{
		res << '<';

		for (auto & ta : id.tpl_args)
		{
			res << boost::trim_copy(ta);
			if (&ta != &id.tpl_args.back())
				res << ',';
		}
		res << '>';
	}
	return res.str();
}


struct obj_future
{
	std::shared_future<data::object_p> target;

	std::string name;

	obj_future * waits_for = nullptr;
};

struct object_transform_buf
{
	//ok, this stores the futures, when several threads are waiting.
	std::unordered_map<std::string, obj_future>		transformed_objects;

	//this is the storage of the finished thingys.
	std::set<data::object_p> 		test_objects;
	std::set<data::object_tpl_p> 	test_object_tpls;

	std::mutex tpls_mtx;
	std::mutex object_mtx;

};




static data::object_tpl_p transform_tpl(object_transform_buf &ob, const ast::test_object &to)
{
	using namespace std;

	data::object_tpl_p p = nullptr;

	switch(to.type)
	{
	case ast::object:
	{
		auto p2 = std::make_shared<data::test_object_tpl>();
		p = p2;
		break;
	}
	case ast::classification:
	case ast::composition:
	case ast::class_:
	case ast::case_:
	case ast::step:
	case ast::sequence:
		break;
	}

	return p;
}

static data::object_tpl_p transform_obj(object_transform_buf &ob, const ast::test_object &to)
{

}

static data::object_p instanciate_template(object_transform_buf & ob, const ast::obj_id & id)
{

}


data::main to_data(const std::vector<std::shared_ptr<mwt_file>> & parsed,
				   const std::vector<boost::filesystem::path>& include_path,
				   std::launch async_mode)
{
	using namespace std;

	auto fut = std::async(async_mode, use_file_transform_all, parsed); //ok, this goes first, beause it maybe async


	//so, the first thing we do, is to collect the templates and the none-template objects

	vector<ast::test_object*> tpls;
	vector<ast::test_object*> raws;

	for (auto & p : parsed)
	{
		for (auto  & o : p->test_objects)
		{
			if (o.is_template())
				tpls.push_back(&o);
			else
				raws.push_back(&o);
		}
	}

	//this is heavily functional style, needs to be better documented
	object_transform_buf ob;

	auto transform_object =
			[&](const ast::test_object & to)
			{

			};

	auto object_transform_lambda =
			[&]{
		/********************************************************************************************************************************/
			};


	std::future<void> object_transform = async(async_mode, object_transform_lambda);

	data::main ret;
	//ok, this stuff can be done whlie the other stuff runs asynchronous
	ret.files = get_files(parsed);

	ret.include_paths.reserve(include_path.size());
	for (auto &inc : include_path)
		ret.include_paths.push_back(inc.string());

	ret.use_files = use_file_transform_all(parsed); // fut.get();


	//ok, finish the object transformation and copy the values from the hash maps.
	object_transform.get();

	ret.test_object_tpls.assign(ob.test_object_tpls.begin(), ob.test_object_tpls.end());
	ret.test_objects 	.assign(ob.test_objects.begin(), 	 ob.test_objects.end());

	return ret;

}

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


	vector<shared_ptr<mwt_file>> ret;

	while (!futures.empty())//ok, always takeout the first and create new futures from the includes.
	{
		auto res = futures.front().get();//ok, get the first.
		futures.erase(futures.begin()); //delete the currently taken.

		ret.push_back(res); //push the found file into the return value.
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

	return ret;

}


}
}
