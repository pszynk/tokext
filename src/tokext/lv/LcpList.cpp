
#include "LcpList.hpp"

#include <assert.h>
#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <prettyprint/prettyprint.h>

namespace tokext {
    namespace lv {

static LoggerPtr loggerLcpUpdate(Logger::getLogger("tokext.lv.LcpList.lcpUpdate"));
static LoggerPtr loggerListUpdate(Logger::getLogger("tokext.lv.LcpList.listUpdate"));

LcpList::LcpList(idx_t flows, idx_t suspicious, idx_t lcpMax, Index& index):
    lcpBound_(lcpMax+1), index_(index), intptr_(lcpBound_+1)
{
    //LOG4CXX_INFO(loggerLcpUpdate, "max lcp " << lcpMax);
    // start -> [(lcp = 1, idxs = {N}), (lcp = 0, idxs = {N-1, N-2, ... 1, 0})]
    intervals_.emplace_back(flows - 1, (suspicious == flows), 1);
    intptr_[1] = --intervals_.end();
    intervals_.emplace_back(0, flows - 2, suspicious - (suspicious == flows), 0);
    intptr_[0] = --intervals_.end();

    this->textptr_.reserve(flows);
    for(auto itv = intervals_.rbegin(); itv != intervals_.rend(); ++itv) {

        for(auto itw = itv->rbegin(); itw != itv->rend(); ++itw) {
            textptr_.push_back(--itw.base());
        }
    }
}


idx_t LcpList::idxOfSuffix(idx_t textIdx) const
{
    return *textptr_[textIdx];
}

void LcpList::lcpUpdate(idx_t lcp)
{
    idx_t k = 0;
    iterator last = intervals_.end();
    auto itv = intervals_.begin();

    //LOG4CXX_INFO(loggerLcpUpdate, "lcp " << lcp);

    LOG4CXX_DEBUG(loggerLcpUpdate,// {{{
            "START ---------------------------------------------------------"
            << "\nLCP = " << lcp
            <<"\n---------------------------------------------------------------");// }}}

    while (itv != intervals_.end()) {
        LOG4CXX_DEBUG(loggerLcpUpdate,// {{{
                "========================================================="
                << "\nList = " << (*this)
                <<"\n=========================================================");// }}}
        if (itv->empty() && itv->lcp() == 0) {
            LOG4CXX_DEBUG(loggerLcpUpdate, ">>> EMPTY INTERVAL, 0 LCP");
            // LCP of this interval surely changed so we can correct intptr
            // arr (not mandatory)
            //intptr_[itv->lcp()] = NULL; TODO ---
            itv = intervals_.erase(itv);
            continue;
        }

        if (itv->lcp() < lcp) {
            LOG4CXX_DEBUG(loggerLcpUpdate, ">>> BREAK LOOP - [interval lcp = "// {{{
                    << itv->lcp() <<"] < [LCP = " << lcp << "]");// }}}
            break;
        }
        LOG4CXX_DEBUG(loggerLcpUpdate, ">>> SUBSTRING FOUND");
        // if interval is empty, remove it

        // update offset by interval size
        //k += itv->size();

        LOG4CXX_DEBUG(loggerLcpUpdate, ">>> >>>\nk = " << k << "\ninterval = "// {{{
                << (*itv));// }}}


        // concatenate current interval with the last one (and create new last)
        if(last != intervals_.end()) {
            //LOG4CXX_DEBUG(loggerLcpUpdate, ">>> >>> append intervals: "<<(*itv) << " .. " << (*last));
            itv->prepend(*last);
            // remove now empty interval
            intervals_.pop_front();
        }

        // this is new last interval
        last = itv;

        // LCP of this interval surely changed so we can correct intptr
        // arr (not mandatory)
        //intptr_.erase(itv->lcp());

        index_.add(*itv);

        ++itv;
    }

    // adjust new last interval and intptr arr
    if(last != intervals_.end()) {
        //LOG4CXX_DEBUG(loggerLcpUpdate, "adjust last interval: "<<(*last) << " --- lcp: " << lcp);
        //last->save(k);
        last->setLcp(lcp);
        intptr_[lcp] = last;
    }

    LOG4CXX_DEBUG(loggerLcpUpdate,// {{{
            "END -----------------------------------------------------------"
            << "\nLCP = " << lcp
            << "\nList = " << (*this)
            <<"\n---------------------------------------------------------------");// }}}

}

void LcpList::listUpdate(
        idx_t delIntLcp, idx_t delTextIdx, bool delIsSuspicious,
        idx_t putIdx, idx_t putLcp, bool putIsSuspicious)
{
    putLcp = std::min(lcpBound_, putLcp);
    //LOG4CXX_INFO(loggerLcpUpdate, "del lcp " << delIntLcp <<" put lcp " << putLcp);
    LOG4CXX_DEBUG(loggerListUpdate,// {{{
            "BEFORE LIST UPDATE --------------------------------------------"
            << "\ndelete text with idx = " << delTextIdx
            << "\nlcp of interval with text = " << delIntLcp
            << "\nidx of new suffix = " << putIdx
            << "\nlcp of new suffix = " << putLcp
            << "\n\nList = " << (*this)
            <<"\n---------------------------------------------------------------");// }}}

    assert(delIntLcp == intptr_[delIntLcp]->lcp());
    intptr_[delIntLcp]->remove(textptr_[delTextIdx], delIsSuspicious);
    //if (intptr_[delIntLcp]->size() == 0) {
        //intervals_.erase(intptr_[delIntLcp]);
        //intptr_.erase(delIntLcp);
    //}
    intervals_.emplace_front(putIdx, putIsSuspicious, putLcp);
    auto it = intervals_.front().begin();
    textptr_[delTextIdx] = it;
    intptr_[putLcp] = intervals_.begin();

    LOG4CXX_DEBUG(loggerListUpdate,// {{{
            "AFTER LIST UPDATE ---------------------------------------------"
            << "\nList = " << (*this)
            <<"\n---------------------------------------------------------------");// }}}
}

std::ostream& operator<<(std::ostream &strm , const LcpList &lcpList)
{
    std::string indent = "  ";

    strm << "LcpList{" << std::endl
        << indent <<  "          length: "<< lcpList.size() << std::endl
        << indent <<  "  interval count: "<< lcpList.intptr_.size() << std::endl;

    strm << indent << "          intptr: <" << std::endl;
    //for(const auto &p : lcpList.intptr_) {
    //    strm << indent << indent << "{" << p.first << "} -> ((";
    //    if (p.second == lcpList.intervals_.end()) {
    //        strm << "[END]";
    //    } else {
    //        strm << (*p.second);
    //    }
    //    strm << "))" << std::endl;
    //}
    strm << indent << "                  >" << std::endl;
    strm << indent << "       intervals: <" << std::endl;

    idx_t counter = 0;
    for(const auto &inter : lcpList.intervals_) {
        strm << indent << indent << "[" << counter++
            <<"] (lcp: " << inter.lcp() <<")-> [";
        for(auto it = inter.begin(); it != inter.end(); ++it) {
            strm << (*it) << ", ";
        }
        strm << "]" << std::endl;
    }
    strm << indent << "                 >" << std::endl;
    strm << "}" ;

    return strm;
}

    } /* lv */
} /* tokext */
