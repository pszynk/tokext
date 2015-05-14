
#include "extraction.hpp"

#include "../SuffixContext.hpp"
#include "../ExtractionParams.hpp"
#include "../rmq/RmqWrapper.hpp"
#include "../lv/LcpList.hpp"

#include <prettyprint/prettyprint.h>

#include <log4cxx/logger.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <algorithm>

namespace tokext
{
    namespace utils
    {

namespace
{
static LoggerPtr logger(Logger::getLogger("tokext.extractTokens"));
}

std::vector<Token> extractTokens(
        const SuffixContext& context,
        ExtractionParams& params)
{
    LOG4CXX_INFO(logger, "Token extraction started...");

    rmq::RmqWrapper wrap(context.lcpArray());
    lv::Index index(params);
    lv::LcpList lcpList(context.totalFlowCount(),
            context.suspiciousFlowCount(), context.lcpMax(), index);

    for(idx_t i = context.totalFlowCount(); i < context.size(); ++i) {

        idx_t lcp = context.lcp(i);
        lcpList.lcpUpdate(lcp);

        idx_t text = context.text(i);
        idx_t delSuffixIdx = lcpList.idxOfSuffix(text);

        idx_t delIntLcpIdx = wrap.query(delSuffixIdx + 1, i);

        idx_t delIntLcp = context.lcp(delIntLcpIdx);

        idx_t preffixLen = context.textPrefixLength(i);

        lcpList.listUpdate(
                delIntLcp, text, context.isSuspicious(delSuffixIdx),
                i, preffixLen, context.isSuspicious(i));
    }

    // last update to add what's left in the lcp_list
    lcpList.lcpUpdate(0);

    LOG4CXX_INFO(logger, "Token extraction complete [OK]");


    std::vector<Token> tokens;
    idx_t prevIdx = 0;
    for(auto& byIdx : index) {
        idx_t lastLen = std::max(context.lcp(wrap.query(prevIdx, byIdx.first)) + 1, params.reqLength());
        prevIdx = byIdx.first + 1;
        for(auto& byLen : byIdx.second) {
            for(idx_t len = lastLen; len <= byLen.first; ++len) {
                tokens.push_back(Token(len, byLen.second));
            }
            lastLen = byLen.first + 1;
        }
    }

    return tokens;
}
    } /* utils */
} /* tokext */
