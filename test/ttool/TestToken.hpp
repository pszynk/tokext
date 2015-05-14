
#ifndef TESTTOKEN_HPP_P4VPEVUW
#define TESTTOKEN_HPP_P4VPEVUW

#include "tokext/types.hpp"
#include "tokext/Occurrence.hpp"

#include <prettyprint/prettyprint.h>

#include <vector>
#include <ostream>

namespace ttool
{
class TestToken
{
public:
    using buffer_t = std::vector<tokext::ichar_t>;
    TestToken(tokext::Occurrence occ_, buffer_t data_);
private:
    tokext::Occurrence occurrence_;
    buffer_t data_;

    friend bool operator==(const TestToken &lhs, const TestToken &rhs);
    friend std::ostream& operator<<(std::ostream &strm, const TestToken &ttoken);
};


inline
bool operator==(const TestToken &lhs, const TestToken &rhs)
{
    return (lhs.occurrence_ == rhs.occurrence_) && (lhs.data_ == rhs.data_);
}

inline
std::ostream& operator<< (std::ostream &strm, const TestToken &ttoken) {
   return strm << "<" << ttoken.occurrence_ << " -> "
       << ttoken.data_ << ">" << std::endl;
 }

inline
TestToken::TestToken(tokext::Occurrence occurrence, buffer_t data):
    occurrence_(occurrence), data_(data) {}

} /* ttool */
#endif /* end of include guard: TESTTOKEN_HPP_P4VPEVUW */
