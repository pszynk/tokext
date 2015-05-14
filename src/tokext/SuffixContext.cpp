#include "SuffixContext.hpp"

#include "InputData.hpp"

#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <algorithm>
#include <stdexcept>

namespace tokext
{

namespace
{
static LoggerPtr logger(Logger::getLogger("tokext.SuffixContext"));
}

SuffixContext::SuffixContext(const InputData& indata):
    ibuffer_(indata), gsa_(ibuffer_)
    {
        if(indata.empty()) {
            throw std::invalid_argument("tokext::SuffixContext: no data passed"
                    " to build suffix context");
        }
        LOG4CXX_INFO(logger, "SuffixContext created... [OK]")
    }

std::ostream& operator<<(std::ostream& strm, const SuffixContext& scx)
{
    std::string indent = "  ";

    return strm << "SuffixContext{" << std::endl
        << indent << "  integrated: "<< scx.ibuffer_ << std::endl
        << indent << "suffix array: "<< scx.gsa_ << std::endl;

    return strm;
}

} /* tokext */
