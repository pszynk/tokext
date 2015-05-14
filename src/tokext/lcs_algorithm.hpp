
#ifndef LCSALGORITHM_HPP_4BIR6QB2
#define LCSALGORITHM_HPP_4BIR6QB2


#include "rmq/RmqWrapper.hpp"
//#include "generalized_suffix_array.hpp"
#include "lv/LcpList.hpp"

#include "types.hpp"

namespace tokext
{


class LcsAlgorithm
{
public:
    //LcsAlgorithm(tx_flows_t &flows, idx_t len, idx_t occ);
    //virtual ~LcsAlgorithm ();

    //void process();


private:

    //GeneralizedSuffixArray gsa_;
    //rmq::RmqWrapper rmq_;
    //lv::LcpList lcpList_;


    friend std::ostream& operator<<(std::ostream&, const LcsAlgorithm&);
};

std::ostream& operator<<(std::ostream&, const LcsAlgorithm&);

} /* tokext */

#endif /* end of include guard: LCSALGORITHM_HPP_4BIR6QB2 */

