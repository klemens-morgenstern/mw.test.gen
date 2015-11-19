/**
 * @file   comment.hpp
 * @date   18.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_DATA_DOC_HPP_
#define MW_TEST_DATA_DOC_HPP_

namespace mw
{
namespace test
{
namespace data
{

struct doc
{
	std::string head;
	std::string body;
};

inline doc& operator+=(doc & d, doc& rhs)
{
	d.head += rhs.head;
	d.body += rhs.body;
	return d;
}

}
}
}
#endif /* MW_TEST_DATA_DOC_HPP_ */
