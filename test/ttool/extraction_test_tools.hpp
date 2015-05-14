#ifndef EXTRACTION_TEST_TOOLS_HPP_TCCJ8FAR
#define EXTRACTION_TEST_TOOLS_HPP_TCCJ8FAR

#include "TestToken.hpp"

#include "tokext/types.hpp"
#include "tokext/Occurrence.hpp"

#include <vector>

namespace tokext
{
class InputData;
class SuffixContext;
class ExtractionParams;
} /* tokext */

namespace ttool
{

std::vector<TestToken> naiveTokenExtraction(
        const tokext::InputData& idata,
        tokext::idx_t minLen, tokext::Occurrence minOcc);


std::vector <TestToken> smartTokenExtraction(
        const tokext::SuffixContext& scx, tokext::ExtractionParams& extParams);
} /* ttool */

#endif /* end of include guard: EXTRACTION_TEST_TOOLS_HPP_TCCJ8FAR */
