#ifndef GENERALIZED_SUFFIX_ARRAY_HPP_AO2DIB4G
#define GENERALIZED_SUFFIX_ARRAY_HPP_AO2DIB4G

#include "types.hpp"

#include <string>
#include <iostream>
#include <assert.h>


namespace tokext {

class IntegratedBuffer;

class GeneralizedSuffixArray
{
private:
    typedef std::vector<idx_t> vec_t;
public:
    GeneralizedSuffixArray(const IntegratedBuffer& ibuffer);

public:
    // GETTERS & SETTERS {{{2
    idx_t size() const {
        return this->suffixArray_.size();
    }

    idx_t lcpMax() const {
        return lcpMax_;
    }

    idx_t suffix(idx_t idx) const {
        assert(idx < this->size());
        return this->suffixArray_[idx];
    }

    //const vec_t &suffixArray() const {
    //    return this->suffixArray_;
    //}

    //const vec_t &invSuffixArray() const {
    //    return this->invSuffixArray_;
    //}


    idx_t lcp(idx_t idx) const {
        assert(idx < this->size());
        return this->lcpArray_[idx];
    }

    const vec_t &lcpArray() const {
        return this->lcpArray_;
    }

    flow_idx_t text(idx_t idx) const {
        assert(idx < this->size());
        return this->textArray_[idx];
    }

    //const std::vector<flowidx_t> &textArray() const {
    //    return this->textArray_;
    //}

    //2}}}

private:
    idx_t lcpMax_;
    // suffix array
    // sa_idx -> buff_idx
    vec_t suffixArray_;

    // inversed suffix array
    // buff_idx -> sa_idx
    vec_t invSuffixArray_;

    // longest common prefix array
    // sa_idx -> lcp_val
    vec_t lcpArray_;

    // text array
    // sa_idx -> flow number which suffix is a prefix of global suffix sa_idx
    std::vector<flow_idx_t> textArray_;

    friend std::ostream& operator<<(std::ostream&,
            const GeneralizedSuffixArray&);
};

std::ostream& operator<<(std::ostream&, const GeneralizedSuffixArray&);

} /* namespace tokext */

#endif /* end of include guard: GENERALIZED_SUFFIX_ARRAY_HPP_AO2DIB4G */
