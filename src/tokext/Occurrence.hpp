
#ifndef OCCURRENCE_HPP_30SKBMIB
#define OCCURRENCE_HPP_30SKBMIB

#include "types.hpp"
//#include "HazardLevel.hpp"

#include <ostream>
#include <cassert>

namespace tokext
{

class Occurrence {
    public:
        Occurrence(idx_t inSuspicious = 0, idx_t inNormal = 0);

        void transfer(Occurrence& other);

        idx_t inSuspicious() const;
        idx_t inNormal() const;
        idx_t inTotal() const;
        //idx_t inHazardLevel(HazardLevel level) const;

        void setInSuspicious(idx_t value);
        void setInNormal(idx_t value);
        //void setinHazardLevel(HazardLevel level);

        void incInSuspicious(idx_t value = 1);
        void incInNormal(idx_t value = 1);
        void incIn(bool isSuspicious, idx_t value = 1);
        //void incInHazardLevel(HazardLevel level, idx_t value = 1);

        void decInSuspicious(idx_t value = 1);
        void decInNormal(idx_t value = 1);
        void decIn(bool isSuspicious, idx_t value = 1);
        //void decInHazardLevel(HazardLevel level, idx_t value = 1);


    private:
        idx_t inSuspicious_;
        idx_t inNormal_;

        //friend bool operator<(const Occurrence &lhs, const Occurrence &rhs);
        friend std::ostream& operator<<(std::ostream &strm, const Occurrence &occ);
};

bool operator==(const Occurrence &lhs, const Occurrence &rhs);
bool operator!=(const Occurrence &lhs, const Occurrence &rhs);

bool operator<(const Occurrence &lhs, const Occurrence &rhs);
bool operator<=(const Occurrence &lhs, const Occurrence &rhs);

bool operator>(const Occurrence &lhs, const Occurrence &rhs);
bool operator>=(const Occurrence &lhs, const Occurrence &rhs);

std::ostream& operator<<(std::ostream &strm, const Occurrence &occ);

        inline void takeOver(Occurrence& other);

inline Occurrence::Occurrence(idx_t inSuspicious, idx_t inNormal):
    inSuspicious_(inSuspicious), inNormal_(inNormal)
{}

inline void Occurrence::transfer(Occurrence& other)
{
    inSuspicious_ += other.inSuspicious_;
    other.inSuspicious_ = 0;

    inNormal_ += other.inNormal_;
    other.inNormal_ = 0;
}

inline idx_t Occurrence::inSuspicious() const
{
    return inSuspicious_;
}

inline idx_t Occurrence::inNormal() const
{
    return inNormal_;
}

inline idx_t Occurrence::inTotal() const
{
    return inSuspicious() + inNormal();
}

inline void Occurrence::setInSuspicious(idx_t value)
{
    inSuspicious_ = value;
}

inline void Occurrence::setInNormal(idx_t value)
{
    inNormal_ = value;
}

inline void Occurrence::incInSuspicious(idx_t value)
{
    assert(inSuspicious_ + value >= inSuspicious_);
    inSuspicious_ += value;
}

inline void Occurrence::incInNormal(idx_t value)
{
    assert(inNormal_ + value >= inNormal_);
    inNormal_ += value;
}

inline void Occurrence::incIn(bool isSuspicious, idx_t value)
{
    isSuspicious ? incInSuspicious(value) : incInNormal(value);
}

inline void Occurrence::decInNormal(idx_t value)
{
    assert(inNormal_ >= value);
    inNormal_ -= value;
}

inline void Occurrence::decInSuspicious(idx_t value)
{
    assert(inSuspicious_ >= value);
    inSuspicious_ -= value;
}

inline void Occurrence::decIn(bool isSuspicious, idx_t value)
{
    isSuspicious ? decInSuspicious(value) : decInNormal(value);
}

} /* tokext */


#endif /* end of include guard: OCCURRENCE_HPP_30SKBMIB */
