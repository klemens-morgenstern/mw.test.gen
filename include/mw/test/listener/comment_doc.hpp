/**
 * @file   comment_doc.hpp
 * @date   17.11.2015
 * @author Klemens
 *
 * Published under [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0.html)
 */

#ifndef MW_TEST_LISTENER_COMMENT_DOC_HPP_
#define MW_TEST_LISTENER_COMMENT_DOC_HPP_

#include <string>

namespace mw
{
namespace test
{
namespace listener
{

struct comment
{
	std::string before;
	std::string behind;
	bool empty() const;

	void add_before(const std::string& data);
	void add_behind (const std::string& data);
	void clear();
	static comment& get();
};


}
}
}




#endif /* MW_TEST_LISTENER_COMMENT_DOC_HPP_ */
