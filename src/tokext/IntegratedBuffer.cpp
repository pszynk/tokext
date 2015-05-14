
#include "IntegratedBuffer.hpp"

#include "InputData.hpp"

#include <prettyprint/prettyprint.h>
#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <string>
#include <algorithm>


namespace tokext
{

namespace
{
static LoggerPtr logger(Logger::getLogger("tokext.IntegratedBuffer"));
}

IntegratedBuffer::IntegratedBuffer(const InputData& indata) :
    totalFlowCount_(indata.totalFlowCount()),
    suspiciousFlowCount_(indata.suspiciousFlowCount()),
    normalFlowCount_(indata.normalFlowCount()),
    shift_(indata.totalFlowCount() + 1)
{

    beginnings_.reserve(totalFlowCount_);
    idx_t totalLen = 0;
    idx_t index = 1;
    alphabetSize_ = shift_;

    typedef InputData::iterator it_t;
    for (it_t it = indata.beginSuspicious(); it != indata.endSuspicious();
            ++it) {
        beginnings_.push_back(totalLen);
        totalLen += it->size() + 1;

        for (auto c : (*it)) {
            xchar_t xc = c + shift_;
            data_.push_back(xc);
            if (xc > alphabetSize_) {
                alphabetSize_ = xc;
            }
        }
        data_.push_back(index++);
    }

    this->suspDataSize_ = data_.size();

    for (it_t it = indata.beginNormal(); it != indata.endNormal();
            ++it) {

        beginnings_.push_back(totalLen);
        totalLen += it->size() + 1;

        for (auto c : (*it)) {
            xchar_t xc = c + shift_;
            data_.push_back(xc);
            if (xc > alphabetSize_) {
                alphabetSize_ = xc;
            }
        }
        data_.push_back(index++);
    }
    LOG4CXX_INFO(logger, "IntegratedBuffer created... [OK]")
}

flow_idx_t IntegratedBuffer::flowGlobalIdx(idx_t idx) const
{
    auto up = std::upper_bound(beginnings_.begin(), beginnings_.end(), idx);
    return up - beginnings_.begin() - 1;
}

bool IntegratedBuffer::isSuspicious(flow_idx_t idx) const
{
    return idx < suspiciousFlowCount();
}

std::ostream& operator<<(std::ostream& strm, const IntegratedBuffer& ibuf)
{
    std::string indent = "  ";

    return strm << "IntegratedBuffer{" << std::endl
        << indent << "               size: "<< ibuf.size() << std::endl
        << indent << "     totalFlowCount: "<< ibuf.totalFlowCount() << std::endl
        << indent << "suspiciousFlowCount: "<< ibuf.suspiciousFlowCount() << std::endl
        << indent << "    normalFlowCount: "<< ibuf.normalFlowCount() << std::endl
        << indent << "              shift: "<< ibuf.shift_ << std::endl
        << indent << "         beginnings: "<< ibuf.beginnings_ << std::endl
        << indent << "               data: "<< ibuf.data_ << std::endl;
}

} /* tokext */

