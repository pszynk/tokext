
#ifndef INTERVAL_HPP_80SR2AWL
#define INTERVAL_HPP_80SR2AWL


#include <list>
#include <ostream>

#include "../types.hpp"
#include "../Occurrence.hpp"

namespace tokext
{
class Token;
}

namespace tokext {
    namespace lv {


class Interval
{
private:
    typedef std::list<idx_t> list__;

public:
    Interval(idx_t idx, bool isSuspicious, idx_t lcp);
    Interval(idx_t from, idx_t to, idx_t suspicious, idx_t lcp);

    typedef list__::iterator iterator;
    typedef list__::const_iterator const_iterator;
    typedef list__::reverse_iterator reverse_iterator;
    typedef list__::const_reverse_iterator const_reverse_iterator;
    // GETTERS & SETTERS

    // sa idx
    idx_t firstSufIdx() const;
    idx_t lastSufIdx() const;

    // lcp
    idx_t lcp() const;
    void setLcp(idx_t lcp);

    // occurrence
    Occurrence occurrence() const;

    bool empty() const;
    void remove(iterator iter, bool isSuspicious);

    void prepend(Interval &intrv);

    operator Token() const;

    //begin
    iterator begin();
    const_iterator begin() const;
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;

    //end
    iterator end();
    const_iterator end() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;

private:
    idx_t firstSufIdx_;
    idx_t lastSufIdx_;
    idx_t lcp_;

    Occurrence occurrence_;
    list__ items_;

    friend std::ostream& operator<<(std::ostream &strm, const Interval &interval);
};

std::ostream& operator<<(std::ostream &strm, const Interval &interval);

// INLINES {{{1
inline idx_t Interval::firstSufIdx() const
{
   return firstSufIdx_;
}

inline idx_t Interval::lastSufIdx() const
{
   return lastSufIdx_;
}

inline idx_t Interval::lcp() const
{
    return this->lcp_;
}

inline void Interval::setLcp(idx_t lcp)
{
   this->lcp_ = lcp;
}

// counts
inline Occurrence Interval::occurrence() const
{
   return occurrence_;
}

inline bool Interval::empty() const
{
   return this->items_.empty();
}

//begin
inline Interval::iterator Interval::begin()
{
   return this->items_.begin();
}

inline Interval::const_iterator Interval::begin() const
{
   return this->items_.cbegin();
}

inline Interval::reverse_iterator Interval::rbegin()
{
   return this->items_.rbegin();
}

inline Interval::const_reverse_iterator Interval::crbegin() const
{
   return this->items_.crbegin();
}

//end
inline Interval::iterator Interval::end()
{
   return this->items_.end();
}

inline Interval::const_iterator Interval::end() const
{
   return this->items_.cend();
}

inline Interval::reverse_iterator Interval::rend()
{
   return this->items_.rend();
}

inline Interval::const_reverse_iterator Interval::crend() const
{
   return this->items_.crend();
}

// }}}

    } /* lv */
} /* tokext */

#endif /* end of include guard: INTERVAL_HPP_80SR2AWL */
