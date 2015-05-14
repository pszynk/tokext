
#include "Index.hpp"

#include "../SuffixContext.hpp"
#include "../ExtractionParams.hpp"
#include "../rmq/RmqWrapper.hpp"

#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <algorithm>
#include <ostream>
#include <prettyprint/prettyprint.h>


static LoggerPtr logger(Logger::getLogger("tokext.lv.Index.add"));

namespace tokext
{
    namespace lv
    {

Index::Index(ExtractionParams& params): params_(params)
{}



bool Index::shouldReplace_(const Token& old, const Token& arrived) const
{
    return old.lastSufIdx() < arrived.lastSufIdx();
}

bool Index::check_(const Token& token) const
{
    return params_.accept(token);
}

void Index::add_(Token &&token)
{
    auto p = index_[token.firstSufIdx()].
        insert(std::make_pair(token.length(), token));
    if(shouldReplace_(p.second, token)) {
        p.first->second = token;
    }
}

bool Index::add(Token token)
{
    if(check_(token)) {
        LOG4CXX_DEBUG(logger, "Adding token: \n" << token);
        add_(std::move(token));
        return true;
    }

    LOG4CXX_DEBUG(logger, "Rejecting token: \n" << token);
    return false;
}

inline std::ostream& operator<<(std::ostream &strm , const Index &Index)
{
    std::string indent = "  ";

    strm << "Index{" << std::endl
        << indent <<  "params: "<< Index.params_ << std::endl
        << indent <<  " index: "<< Index.index_ << std::endl;

    return strm;
}

    } /* lv */
} /* tokext */
