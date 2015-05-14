
#ifndef SUFFIX_CONTEXT_HPP_UICXMD6N
#define SUFFIX_CONTEXT_HPP_UICXMD6N

#include "IntegratedBuffer.hpp"
#include "GeneralizedSuffixArray.hpp"
#include "Flow.hpp"
#include "Position.hpp"

#include <ostream>

namespace tokext
{

class InputData;

class SuffixContext
{
public:
    SuffixContext(const InputData& indata);
    //virtual ~SuffixContext ();

    //gsa interface
    idx_t size() const;
    idx_t lcpMax() const;

    idx_t suffix(idx_t idx) const;
    //idx_t inv_suffix(idx_t idx) const;
    idx_t lcp(idx_t idx) const;
    idx_t text(idx_t idx) const;

    const std::vector<idx_t>& lcpArray() const;

    //ib interface
    // TODO: jaki? (07/10/14 12:49:55 - pawel)
    //
    idx_t totalFlowCount() const;
    idx_t suspiciousFlowCount() const;
    idx_t normalFlowCount() const;
    void freeNormalData() {
        ibuffer_.freeNormalData();
    }

    IntegratedBuffer::iterator beginIBuffer() const;
    IntegratedBuffer::iterator endIBuffer() const;
    xchar_t shift() const;

    // combined interace
    // dlugosc prefixu przy ograniczeniu do danego tekstu
    idx_t textPrefixLength(idx_t idx) const; // dawniej prefix_length
    // dlugosc safixu przy ograniczeniu do danego tekstu
    idx_t textSufixLength(idx_t idx) const; // dawniej prefix_length
    // czy sufix o danym indeksie nalezy do przeplywu podejrzanego
    bool isSuspicious(idx_t idx) const;

    idx_t prefixOffset(idx_t idx) const;
    Flow prefixFlow(idx_t idx) const;
    Position prefixPosition(idx_t idx) const;

private:
    IntegratedBuffer ibuffer_;
    GeneralizedSuffixArray gsa_;

    friend std::ostream& operator<<(std::ostream&, const SuffixContext&);
};

// gsa
inline idx_t SuffixContext::size() const
{
    return this->gsa_.size();
}

inline
idx_t SuffixContext::lcpMax() const
{
    return this->gsa_.lcpMax();
}

inline idx_t SuffixContext::suffix(idx_t idx) const
{
    return this->gsa_.suffix(idx);
}

//inline idx_t SuffixContext::invSuffix(idx_t idx) const
//{
//    return this->gsa_.invSuffix(idx);
//}

inline idx_t SuffixContext::lcp(idx_t idx) const
{
    return this->gsa_.lcp(idx);
}

inline idx_t SuffixContext::text(idx_t idx) const
{
    return this->gsa_.text(idx);
}

inline const std::vector<idx_t>& SuffixContext::lcpArray() const
{
    return this->gsa_.lcpArray();
}

inline idx_t SuffixContext::totalFlowCount() const
{
    return this->ibuffer_.totalFlowCount();
}

inline idx_t SuffixContext::suspiciousFlowCount() const
{
    return this->ibuffer_.suspiciousFlowCount();
}

inline idx_t SuffixContext::normalFlowCount() const
{
    return this->ibuffer_.normalFlowCount();
}


inline IntegratedBuffer::iterator SuffixContext::beginIBuffer() const
{
    return ibuffer_.begin();
}

inline IntegratedBuffer::iterator SuffixContext::endIBuffer() const
{
    return ibuffer_.end();
}

inline xchar_t SuffixContext::shift() const
{
    return ibuffer_.shift();
}

// sa index -> size
inline idx_t SuffixContext::textPrefixLength(idx_t idx) const
{
    return ibuffer_.flowEnd(gsa_.text(idx)) - gsa_.suffix(idx);
}

inline idx_t SuffixContext::textSufixLength(idx_t idx) const
{
    return gsa_.suffix(idx) - ibuffer_.flowBegin(gsa_.text(idx));
}

inline bool SuffixContext::isSuspicious(idx_t idx) const
{
    return ibuffer_.isSuspicious(text(idx));
}

inline idx_t SuffixContext::prefixOffset(idx_t idx) const
{
    return textSufixLength(idx);
}

inline Flow SuffixContext::prefixFlow(idx_t idx) const
{
    auto gidx = text(idx);
    bool susp = ibuffer_.isSuspicious(gidx);
    auto lidx = (susp ? gidx : ibuffer_.suspiciousFlowCount() - gidx);
    return Flow(susp, lidx, gidx);
}

inline Position SuffixContext::prefixPosition(idx_t idx) const
{
    auto gidx = text(idx);
    bool susp = ibuffer_.isSuspicious(gidx);
    auto lidx = (susp ? gidx : ibuffer_.suspiciousFlowCount() - gidx);
    auto offset = gsa_.suffix(idx) - ibuffer_.flowBegin(gsa_.text(idx));
    return Position(offset, Flow(susp, lidx, gidx));
}

} /* tokext */

#endif /* end of include guard: SUFFIX_CONTEXT_HPP_UICXMD6N */
