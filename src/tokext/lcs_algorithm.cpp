#include "lcs_algorithm.hpp"

#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

namespace tokext
{

    /*
static LoggerPtr loggerLcs(Logger::getLogger("tokext.LcsAlgorithm.process"));

LcsAlgorithm::LcsAlgorithm(tx_flows_t &flows, idx_t len, idx_t occ):
    gsa_(flows), rmq_(gsa_.lcp_array()), lcpList_(gsa_.num_flows(), len, occ)
{
}


void LcsAlgorithm::process()
{
    LOG4CXX_DEBUG(loggerLcs, "lcs:\n\t" << *this);
    for(idx_t i = gsa_.num_flows(); i < gsa_.size(); ++i) {

        idx_t lcp = gsa_.lcp(i);
        lcpList_.lcp_update(lcp);

        idx_t text = gsa_.text(i);
        idx_t delSuffixIdx = lcpList_.idx_of_suffix(text);

        LOG4CXX_DEBUG(loggerLcs, "rmq("<<delSuffixIdx + 1<<", "<<i<<")");
        idx_t delIntLcpIdx = rmq_.rmq(delSuffixIdx + 1, i);
        idx_t delIntLcp = gsa_.lcp(delIntLcpIdx);

        idx_t preffixLen = gsa_.prefix_length(i);

        LOG4CXX_DEBUG(
                loggerLcs,
                "i = " << i <<
                ", lcp = "<< lcp <<
                ", text = "<< text <<
                ", delSuffixIdx = "<< delSuffixIdx <<
                ", delIntLcpIdx = "<< delIntLcpIdx <<
                ", delIntLcp = "<< delIntLcp <<
                ", preffixLen = "<< preffixLen
                );

        lcpList_.list_update(delIntLcp, text, i, preffixLen);
    }

    // last update to add what's left in the lcp_list
    lcpList_.lcp_update(0);

    LOG4CXX_INFO(
            loggerLcs,
            "\n" << gsa_.alnum_table());

    LOG4CXX_INFO(
            loggerLcs,
            "INDEX:\n" << lcpList_.index());

    for (auto p : lcpList_.index().index()) {
        for (auto r : p.second) {
            LOG4CXX_INFO(loggerLcs, "> idx = " << p.first << " occ = " <<
                    r.second << " : " + gsa_.alnum_substr(gsa_.suffix(p.first), r.first));
        }
    }
}


std::ostream& operator<<(std::ostream& strm, const LcsAlgorithm& lcsa)
{
    std::string indent = "  ";

    return strm << indent << "LcsAlgorithm{" << std::endl
        << indent << "general suffix array: " << lcsa.gsa_ << std::endl
        << indent << "            lca list: " << lcsa.lcpList_ << std::endl
        << "}";
}
*/

} /* tokext */
