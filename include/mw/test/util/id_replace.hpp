/**
 * @file   id_replace.hpp
 * @date   02.02.2016
 * @author Klemens D. Morgenstern
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_UTIL_ID_REPLACE_HPP_
#define MW_TEST_UTIL_ID_REPLACE_HPP_

#include <string>
#include <vector>
#include <regex>

namespace mw
{
namespace test
{
namespace util
{

struct replacement
{
    std::string identifier;
    std::string content;
};

std::string id_replace(
        const std::string& s,
        const std::vector<replacement> & replacements)
{

    std::regex    id_regex("[_A-Za-z]\\w*");
    auto words_begin =
           std::sregex_token_iterator(s.cbegin(), s.cend(), id_regex);

    auto words_end = std::sregex_token_iterator();

    std::string ret;
    ret.reserve(s.size());

    auto last_inserted = s.cbegin();


    for (auto & v : boost::make_iterator_range(words_begin, words_end))
    {
        if (!v.matched)
            continue;

        auto itr = std::find_if(replacements.begin(), replacements.end(),
                [&](const replacement &r)
                {
                    return r.identifier == v;
                });
        if (itr == replacements.end())
            continue;

        //ok, got a match
        ret.append(last_inserted, v.first);
        ret.append(itr->content);
        last_inserted = v.second;
    }
    ret.append(last_inserted, s.cend());

    return ret;
}


}
}
}


#endif /* MW_TEST_UTIL_ID_REPLACE_HPP_ */
