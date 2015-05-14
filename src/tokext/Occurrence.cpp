
#include "Occurrence.hpp"

namespace tokext
{

bool operator==(const Occurrence &lhs, const Occurrence &rhs)
{
    return (lhs.inSuspicious() == rhs.inSuspicious())
        && (lhs.inNormal() == rhs.inNormal());
}

bool operator!=(const Occurrence &lhs, const Occurrence &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Occurrence &lhs, const Occurrence &rhs)
{
    return (lhs.inSuspicious() < rhs.inSuspicious())
        || (!(lhs.inSuspicious() < rhs.inSuspicious())
                && lhs.inNormal() < rhs.inNormal());
}

bool operator<=(const Occurrence &lhs, const Occurrence &rhs)
{
    return !(rhs < lhs);
}

bool operator>(const Occurrence &lhs, const Occurrence &rhs)
{
    return rhs < lhs;
}

bool operator>=(const Occurrence &lhs, const Occurrence &rhs)
{
    return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream &strm, const Occurrence &occ)
{
    strm << "Occurrence{"
        << occ.inSuspicious() << "/"
        << occ.inNormal() << "|"
        << occ.inTotal() << "}";
    return strm;
}

} /* tokext */
