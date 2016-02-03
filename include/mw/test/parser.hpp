/**
 * @file   parser.hpp
 * @date   29.01.2016
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_PARSER_HPP_
#define MW_TEST_PARSER_HPP_

#include <stack>
#include <set>
#include <mw/test/mwt_file.hpp>
#include <mw/test/data/main.hpp>


namespace mw
{
namespace test
{

namespace data
{
struct use_file;
struct group;
}

/*
 *
 */
struct parser
{
    parser() ;

    typedef mwt_file::iterator iterator;

    std::set<boost::filesystem::path> already_parsed;
    std::stack<mwt_file> include_stack;

    //used for the template creation
    data::doc_t pre_doc;

    data::main main_data;

    mwt_file tpl_inst_file;
    mwt_file & current_file() {return include_stack.top();}
    static parser &instance();

    data::object_p get_object(const data::obj_id&);

    data::object_p instanciate_template(
                const std::vector<std::string> & tpl_args,
                const data::object_tpl_p & tpl,
                const std::string & hash);

    data::object& make_object(
                const boost::typeindex::type_index & type,
                const data::location & loc,
                const std::string & id,
                const std::vector<data::obj_id> & inheritance,
                const std::vector<data::object_content>  & obj_cont,
                const data::doc_t & doc);

    data::object_tpl& register_template(
            const boost::typeindex::type_index & type,
            const data::location & loc,
            const std::string & id,
            const std::vector<data::tpl_arg> & tpl_arg,
            const std::vector<data::obj_id> & inheritance,
            const data::code & obj_cont,
            const data::doc_t & doc);

    void add_use_file(const data::use_file &uf);

    void add_group(const data::group & grp);

    void include(const boost::filesystem::path & p, const data::location & loc = data::location());

    void parse(const iterator & begin, const iterator & end);

    void parse_file(const boost::filesystem::path & p);
private:
    std::vector<boost::filesystem::path> _include_paths;


};

} /* namespace test */
} /* namespace mw */

#endif /* MW_TEST_PARSER_HPP_ */
