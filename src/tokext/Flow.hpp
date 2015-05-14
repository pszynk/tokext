
#ifndef FLOW_HPP_KJTECBV4
#define FLOW_HPP_KJTECBV4

#include "types.hpp"

namespace tokext
{

class Flow
{
public:
    typedef flow_idx_t idx_t;

    Flow(bool isSuspicious, idx_t localIdx, idx_t globalIdx):
        isSuspicious_(isSuspicious), localIdx_(localIdx), globalIdx_(globalIdx)
    {};

    bool isSuspicious() const { return isSuspicious_; };
    idx_t localIdx() const { return localIdx_; };
    idx_t globalIdx() const { return globalIdx_; };

private:
    const bool isSuspicious_;
    const flow_idx_t localIdx_;
    const flow_idx_t globalIdx_;
};

} /* tokext */

#endif /* end of include guard: FLOW_HPP_KJTECBV4 */
