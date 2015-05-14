
#include "TokenUtility.hpp"

#include "InputData.hpp"
#include "utils/extraction.hpp"

namespace tokext

{
TokenUtility::TokenUtility(const InputData& idata):
    suffixContext_(idata)
{}

std::vector<Token> TokenUtility::extractTokens(ExtractionParams& params)
{
    return utils::extractTokens(this->suffixContext_, params);
}

} /* tokext */
