#include "Interval.hpp"

#include "../Token.hpp"

#include <cassert>
#include <iostream>

#include <prettyprint/prettyprint.h>


namespace tokext {
    namespace lv {

Interval::Interval(idx_t idx, bool isSuspicious, idx_t lcp):
    firstSufIdx_(idx), lastSufIdx_(idx),
    lcp_(lcp),
    occurrence_(isSuspicious, 1 - isSuspicious)
{
    items_.push_back(idx);
}

Interval::Interval(idx_t from, idx_t to, idx_t suspicious, idx_t lcp):
    firstSufIdx_(from), lastSufIdx_(to),
    lcp_(lcp),
    occurrence_(suspicious, (to - from + 1) - suspicious)
{
    assert(from <= to);
    assert((to - from + 1) >= suspicious);

    idx_t curr = to;
    while (curr != from) {
        items_.push_back(curr);
        curr -= 1;
    }
    items_.push_back(from);

};

void Interval::remove(iterator iter, bool isSuspicious)
{
    assert(items_.size() > 0);
    items_.erase(iter);
    isSuspicious ? occurrence_.decInSuspicious() : occurrence_.decInNormal();
}

void Interval::prepend(Interval &intrv)
{
    items_.splice(begin(), intrv.items_);
    occurrence_.transfer(intrv.occurrence_);
    assert(lastSufIdx_ <= intrv.lastSufIdx());
    lastSufIdx_ = intrv.lastSufIdx();
}

Interval::operator Token() const
{
    return Token(firstSufIdx_, lastSufIdx_, lcp_, occurrence_);
}

std::ostream& operator<<(std::ostream &strm , const Interval &interval)
{

    strm << "Interval{"
        << "   first idx: " << interval.firstSufIdx_
        << "    last idx: " << interval.lastSufIdx_
        << "         lcp: " << interval.lcp()
        << "  occurrence: " << interval.occurrence()
        << "        idxs: " << interval.items_;
        strm << "}";
    return strm;
}

    } /* lv */
} /* tokext */
