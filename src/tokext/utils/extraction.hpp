
#ifndef EXTRACTION_HPP_1KNCT49S
#define EXTRACTION_HPP_1KNCT49S

#include "../Token.hpp"

namespace tokext
{

class SuffixContext;

class ExtractionParams;

    namespace utils
    {

std::vector<Token> extractTokens(
        const SuffixContext& context,
        ExtractionParams& constraint);

    } /* utils */
} /* tokext */

#endif /* end of include guard: EXTRACTION_HPP_1KNCT49S */
