/**
 * @file   comment.hpp
 * @date   18.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_DOCT_HPP_
#define MW_TEST_DATA_DOCT_HPP_

namespace mw
{
namespace test
{
namespace data
{

struct doc_t
{
	std::string head;
	std::string body;
};

inline doc_t& operator+=(doc_t & d, doc_t& rhs)
{
	d.head += rhs.head;
	d.body += rhs.body;
	return d;
}

}
}
}
#endif /* MW_TEST_DATA_DOCT_HPP_ */
