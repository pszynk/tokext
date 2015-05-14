
#include "GeneralizedSuffixArray.hpp"

#include "IntegratedBuffer.hpp"
#include "skew.hpp"

#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include <prettyprint/prettyprint.h>
#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;


namespace tokext {

namespace
{

static LoggerPtr logger(Logger::getLogger("tokext.GeneralizedSuffixArray"));

using vec_t = std::vector<idx_t>;

vec_t computeInvSa(const vec_t& suf)
{
    vec_t inv(suf.size());
    for(idx_t i = 0; i < suf.size(); ++i) {
        inv[suf[i]] = i;
    }
    return inv;
}

std::pair<idx_t, vec_t> computeLcp(const vec_t& suf, const vec_t& inv, const IntegratedBuffer ibuff)
{
    vec_t lcp(suf.size());

    idx_t j = 0, k = 0, l = 0, biggest = 0;

    for (idx_t i = 0; i < suf.size(); ++i) {
        if( (j = inv[i]) ) {
            k = suf[j-1];
            while(ibuff[k+l] == ibuff[i+l]) {
                ++l;
            }
            lcp[j]=l;
            biggest = std::max(biggest, l);
            l = l ? (l-1) : 0;
        }
    }
    return std::make_pair(biggest, std::move(lcp));
}

std::vector<flow_idx_t> computeText(const vec_t& suf, const vec_t& inv, const IntegratedBuffer ibuff)
{
    std::vector<flow_idx_t> text(suf.size() + 1);
    // tmp guard on flow_begin
    //flowBegin_.push_back(suffix_array_.size());


    for(idx_t i = 0; i < ibuff.size(); ++i) {
        text[inv[i]] = ibuff.flowGlobalIdx(i);
    }
    //flow_idx_t index = 0;
    //idx_t j=0;
    //for (idx_t i = 0; i < ibuff.total_count(); i++) {
    //    idx_t next_flowbegin = flowBegin_[i+1];
    //    for(; j<next_flowbegin; j++) {
    //        text_array_[inv_suffix_array_[j]] = index;
    //    }
    //    index++;
    //}

    // remove tmp guard on flow_begin
    //flowBegin_.pop_back();
    // guard
    //text_array_[suffix_array_.size()]=0;

    return text;

}
}

GeneralizedSuffixArray::GeneralizedSuffixArray(const IntegratedBuffer& ibuffer):
    suffixArray_(
            skew::skewSuffixArray(
                skew::vec_t(ibuffer.begin(), ibuffer.end()), // TODO: jakoś inaczej z implicit? (07/10/14 15:57:10 - pawel)
                ibuffer.alphabetSize())),
    invSuffixArray_(computeInvSa(suffixArray_)),
    textArray_(computeText(suffixArray_, invSuffixArray_, ibuffer))
{
    auto lcpTmp = computeLcp(suffixArray_, invSuffixArray_, ibuffer);
    lcpMax_ = lcpTmp.first;
    lcpArray_ = std::move(lcpTmp.second);
    LOG4CXX_INFO(logger, "GeneralizedSuffixArray created... [OK]")
};

std::ostream& operator<<(std::ostream &strm , const GeneralizedSuffixArray &gsa)
{
    std::string indent = "  ";

    return strm << "GeneralizedSuffixArray{" << std::endl
        << indent << "          size: "<< gsa.size() << std::endl
        << indent << "   suffixArray: "<< gsa.suffixArray_ << std::endl
        << indent << "invSuffixArray: "<< gsa.invSuffixArray_ << std::endl
        << indent << "      lcpArray: "<< gsa.lcpArray_ << std::endl
        << indent << "          text: "<< gsa.textArray_ << std::endl;
}

} /* namespace tokext */
