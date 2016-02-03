/**
 * @file   parser.cpp
 * @date   29.01.2016
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#include <cassert>
#include <mw/test/parser.hpp>
#include <mw/test/util/error_log.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/tuple/tuple.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/uuid/sha1.hpp>

namespace mw
{
namespace test
{

thread_local parser *_instance = nullptr;


parser::parser()
{
    assert(_instance == nullptr);

    _instance = this;
}


parser &parser::instance()
{
    assert(_instance != nullptr);
    return *_instance;
}


/* *** Object Load stuff *** */

template<typename T>
std::string concat_names()
{
    return data::object_name<T>;
}

template<typename First, typename Second, typename ...Args>
std::string concat_names()
{
    return data::object_name<First> + std::string(", ") + concat_names<Second, Args...>();
}


template<typename T>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par, void*)
{
    //check it does not inherit anything
    if (inheritance.size() > 0)
    {
        util::error(loc) << data::object_name<T> << " " << id << " cannot inherit anything" << std::endl;
        exit(1);
    }
    return std::make_shared<T>();
}

template<typename T, typename Father>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par, std::weak_ptr<Father>*)
{
    if (inheritance.size() != 1)
    {
        util::error(loc) << data::object_name<T> << " " << id << " does not inherit anything, but must inherit a " << data::object_name<Father> << std::endl;
        exit(1);
    }

    auto p = std::make_shared<T>();

    auto father = std::dynamic_pointer_cast<Father>(par.get_object(inheritance[0]));

    if (!father )
    {
        util::error(loc) << data::object_name<T> << " " << inheritance[0].name << " inherited by " << id << ", but is not " << data::object_name<Father> << std::endl;
        exit(1);
    }

    p->father = father;



    return p;
}

template<typename T, typename ...Fathers>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par, boost::variant<std::weak_ptr<Fathers>...>*)
{
    if (inheritance.size() != 1)
    {
        util::error(loc) << data::object_name<T> << " " << id <<" must inherit exactly one of either of these types: "
                << concat_names<Fathers...>() << std::endl;
        exit(1);
    }

    auto p = std::make_shared<T>();


    auto father_candidate = par.get_object(inheritance[0]);

    bool found = false;
    struct canceler {};
    try {
        boost::fusion::for_each(boost::fusion::tuple<Fathers*...>(),
            [&](auto x) {
                using FT = std::remove_pointer_t<decltype(x)>;

                auto pp = std::dynamic_pointer_cast<FT>(father_candidate);
                if (pp)
                {
                    found = true;
                    p->father = std::move(pp);
                    throw canceler();
                }
            });
    } catch (canceler&) {};

    if (!found)
    {
        util::error(loc) << data::object_name<T> << " " << id
                         << " inherits " << inheritance[0].name << " which is not one the allowed types, which are: "
                         << concat_names<Fathers...>() << std::endl;
        std::exit(1);
    }



    return p;
}

template<typename T, typename Father>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par, std::vector<std::weak_ptr<Father>>*)
{
    if (inheritance.size() != 1)
    {
        util::error(loc) << data::object_name<T> << " " << id <<" can only inherit these types: " <<  data::object_name<Father> << std::endl;
        exit(1);
    }

    auto p = std::make_shared<T>();
    for (auto & f_id : inheritance)
    {
        auto father = std::dynamic_pointer_cast<Father>(par.get_object(inheritance[0]));

        if (!father )
        {
            util::error(loc) << data::object_name<T> << " " << inheritance[0].name << " inherited by " << id << ", but is not " << data::object_name<Father> << std::endl;
            exit(1);
        }

        p->fathers.push_back(std::move(father));

    }
    return p;
}

template<typename T, typename ...Fathers>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par, std::vector<boost::variant<std::weak_ptr<Fathers>...>>*)
{
    if (inheritance.size() != 1)
    {
        util::error(loc) << data::object_name<T> << " " << id <<" must inherit exactly one of either of these types: " << concat_names<Fathers...>() << std::endl;
        exit(1);
    }

    auto p = std::make_shared<T>();
    for (auto & f_id : inheritance)
    {
        auto father_candidate = par.get_object(inheritance[0]);

           bool found = false;
           struct canceler {};
           try {
               boost::fusion::for_each(boost::fusion::tuple<Fathers*...>(),
                   [&](auto x) {
                       using FT = std::remove_pointer_t<decltype(x)>;

                       auto pp = std::dynamic_pointer_cast<FT>(father_candidate);
                       if (pp)
                       {
                           found = true;
                           p->fathers.push_back(std::move(pp));
                           throw canceler();
                       }
                   });
           } catch (canceler&) {};

           if (!found)
           {
               util::error(loc) << data::object_name<T> << " " << id
                                << " inherits " << f_id.name << " which is not one the allowed types, which are: "
                                << concat_names<Fathers...>() << std::endl;
               std::exit(1);
           }
    }

    return p;
}

template<typename T>
inline data::object_p make_object(const std::string & id, const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & par)
{
    using f_p = typename T::father_t*;
    return make_object<T>(id, inheritance, loc, par, f_p());
}


inline data::object_p select_type(const boost::typeindex::type_index & type, const std::string &id,  const std::vector<data::obj_id> & inheritance, const data::location & loc, parser & parser)
{
    using boost::typeindex::type_id;

    data::object_p p;

    if      (type == type_id<data::test_object        >())  p = make_object<data::test_object        >(id, inheritance, loc, parser);
    else if (type == type_id<data::test_classification>())  p = make_object<data::test_classification>(id, inheritance, loc, parser);
    else if (type == type_id<data::test_composition   >())  p = make_object<data::test_composition   >(id, inheritance, loc, parser);
    else if (type == type_id<data::test_class         >())  p = make_object<data::test_class         >(id, inheritance, loc, parser);
    else if (type == type_id<data::test_case          >())  p = make_object<data::test_case          >(id, inheritance, loc, parser);
    else if (type == type_id<data::test_step          >())  p = make_object<data::test_step          >(id, inheritance, loc, parser);
    else if (type == type_id<data::test_sequence      >())  p = make_object<data::test_sequence      >(id, inheritance, loc, parser);
    else
    {
        util::internal_error(__FILE__, __LINE__) << "Invalid value in object type " << type.pretty_name() << std::endl;
        exit(1);
    }
    p->id = id;
    p->loc = loc;
    return p;
}

template<typename T>
void assign_action(data::object & obj, const T &oc)
{
    using a = data::action_t;

    switch (oc.action)
    {
    case a::evaluate:
    {
        if (obj.eval)
        {
            util::error(oc.loc)                  << "Redeclaration of evaluation for " << obj.id   << std::endl;
            util::note(obj.eval->get_location()) << "Previously declared here."     << std::endl; //TODO: maybe add the type-name.
            util::note(oc.loc)                   << "Ignoring the redefinition."    << std::endl;
        }
        else
            obj.eval = oc;
    }
    break;
    case a::execute:
    {
        if (obj.exec)
        {
            util::error(oc.loc)                  << "Redeclaration of execution for " << obj.id   << std::endl;
            util::note(obj.exec->get_location()) << "Previously declared here."     << std::endl; //TODO: maybe add the type-name.
            util::note(oc.loc)                   << "Ignoring the redefinition."    << std::endl;
        }
        else
            obj.exec = oc;
    }
    break;
    case a::initialize:
    {
        if (obj.init)
        {
            util::error(oc.loc)                  << "Redeclaration of initialization for " << obj.id   << std::endl;
            util::note(obj.init->get_location()) << "Previously declared here."     << std::endl; //TODO: maybe add the type-name.
            util::note(oc.loc)                   << "Ignoring the redefinition."    << std::endl;
        }
        else
            obj.init = oc;
    }
    break;
    }
}




data::object& parser::make_object(
            const boost::typeindex::type_index & type,
            const data::location & loc,
            const std::string & id,
            const std::vector<data::obj_id> & inheritance,
            const std::vector<data::object_content>  & obj_cont)
{
    auto itr = std::find_if(main_data.test_objects.begin(), main_data.test_objects.end(),
             [&](const data::object_p& p){return p->id == id;});

    if (itr != main_data.test_objects.end())//template with that id was already declared.
    {
        util::error(loc)   << "Redeclaration of " << id << std::endl;
        util::note((*itr)->loc) << "Previously declared here." << std::endl; //TODO: maybe add the type-name.
        util::note(loc)    << "Ignoring the redefinition." << std::endl;
        return **itr;
    }


    auto p = select_type(type, id, inheritance, loc, *this);

    for (auto & c : obj_cont)
    {
        if (c.is_action()) //might be execute/init/using
            assign_action(*p, c.as_action());
        else if (c.is_using())
            assign_action(*p, c.as_using());
        else if (c.is_functional())
            p->other_content.emplace_back(c.as_functional());
        else
            p->other_content.emplace_back(c.as_code());
    }


    main_data.test_objects.push_back(p);
    return *p;
}

data::object_tpl& parser::register_template(
        const boost::typeindex::type_index & type,
        const data::location & loc,
        const std::string & id,
        const std::vector<data::tpl_arg> & tpl_arg,
        const std::vector<data::obj_id> & inheritance,
        const data::code & obj_cont)
{
    auto itr = std::find_if(main_data.test_object_tpls.begin(), main_data.test_object_tpls.end(),
            [&](const data::object_tpl_p& p){return p->id == id;});

    if (itr != main_data.test_object_tpls.end())//template with that id was already declared.
    {
        util::error(loc)  << "Redeclaration of template " << id << std::endl;
        util::note((*itr)->loc) << "Previously declared here." << std::endl;
        util::note(loc)    << "Ignoring the redifition." << std::endl;
        return **itr;
    }

    auto p = std::make_shared<data::object_tpl>();
    p->id = id;
    p->loc          = loc;
    p->type         = type;
    p->tpl_args     = tpl_arg;
    p->inheritance  = inheritance;
    p->content      = obj_cont;

    return *p;

}


std::vector<std::string> fill_up_template_args(const std::vector<std::string> & ids, const data::object_tpl_p & tpl)
{
    auto tpl_args = ids;

    if (tpl_args.size() < tpl->tpl_args.size()) //too few template arguments
    {
        tpl_args.reserve(tpl->tpl_args.size());

        int i = 0;
        for (i = tpl_args.size(); i < tpl->tpl_args.size(); i++) //increment until the first default argument
        {
            auto & v = tpl->tpl_args[i].default_arg;
            if (v)
                tpl_args.push_back(*v);
            else //here goes the error, not enough (default-)arguments.
            {
                util::error(tpl->loc) << "Too few template arguments (" << tpl_args.size() <<
                        ") provided for " << tpl->id << ", but " << i << "needed." << std::endl;
                std::exit(1);
            }
        }
    }
    return tpl_args;
}


std::string make_template_name(const std::vector<std::string> & tpl_args)
{
    std::string nm = "<";

    for (auto itr = tpl_args.begin(); itr != tpl_args.end();itr++)
    {
        if (itr != tpl_args.begin())
            nm += ',';

        nm += boost::algorithm::trim_copy(*itr);
    }

    return nm;
}


data::object_p parser::get_object(const data::obj_id& id)
{
    if (id.is_template()) //look for a template
    {
        auto itr = std::find_if(main_data.test_object_tpls.cbegin(), main_data.test_object_tpls.cend(), [&](const data::object_tpl_p & tpl){return tpl->id == id.name;});
        if (itr != main_data.test_object_tpls.cend())
        {
            const auto &tpl = *itr;

            if (id.tpl_args.size() > tpl->tpl_args.size()) //too many template arguments
            {
                util::error(tpl->loc) << id.tpl_args.size() << " template arguments provided for "
                                      << tpl->id << ", but only takes " << tpl->tpl_args.size() << "." << std::endl;
                std::exit(1);
            }

            auto tpl_args = fill_up_template_args(id.tpl_args, tpl);
            auto par_str  = make_template_name(tpl_args);
            using boost::uuids::detail::sha1;
            sha1 s;
            s.process_bytes(par_str.c_str(), par_str.size());
            std::remove_reference_t<sha1::digest_type> digest;
            s.get_digest(digest);

            std::stringstream ss; ss << std::hex;
            for (auto & i : digest)
                ss << i;

            auto hash = ss.str();

            auto _id = id.name + hash;

            //alright, look for the id with hash.
            auto itr2 = std::find_if(main_data.test_objects.cbegin(), main_data.test_objects.cend(), [&](const data::object_p & obj){return obj->id == _id;});
            if (itr2 != main_data.test_objects.cend())
                return *itr2; //found it

            return instanciate_template(tpl_args, *itr, hash);

        }
        else
        {
            //report the error, and exit.
            util::error(id.loc) << "Requested object template ('" << id.name << "') not found." << std::endl;
            util::print_error_location(id.loc);
            exit(1);
            return nullptr;//compatibility.
        }
    }
    else //not a template. look the thingy up
    {
        auto itr = std::find_if(main_data.test_objects.cbegin(), main_data.test_objects.cend(), [&](const data::object_p & obj){return obj->id == id.name;});
        if (itr != main_data.test_objects.cend())
            return *itr; //found it
        else
        {
            //report the error, and exit.
            util::error(id.loc) << "Requested object ('" << id.name << "') not found." << std::endl;
            util::print_error_location(id.loc);
            exit(1);
            return nullptr;//compatibility.
        }

    }
}

data::object_p parser::instanciate_template(
        const std::vector<std::string>& tpl_args, const data::object_tpl_p& tpl,
        const std::string& hash)
{
}

void parser::add_use_file(const data::use_file& uf)
{
}

void parser::add_group(const data::group& grp)
{
}



void parser::include(const boost::filesystem::path& p)
{
}


} /* namespace test */
} /* namespace mw */
