
#ifndef POSITION_HPP_QED9RTZ7
#define POSITION_HPP_QED9RTZ7

#include "types.hpp"
#include "Flow.hpp"

namespace tokext
{

class Position
{
public:
    typedef tokext::idx_t idx_t;
    Position (idx_t offset, Flow flow):
        offset_(offset), flow_(flow)
    {};

    idx_t offset() const {return offset_;};
    const Flow& flow() const {return flow_;};

private:
    const idx_t offset_;
    const Flow flow_;
};

} /* tokext */

#endif /* end of include guard: POSITION_HPP_QED9RTZ7 */
