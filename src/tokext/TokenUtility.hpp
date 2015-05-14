
#ifndef TOKENUTILITY_HPP_CHV1ONWB
#define TOKENUTILITY_HPP_CHV1ONWB

#include "Token.hpp"
#include "SuffixContext.hpp"
#include "utils/translation.hpp"

#include <vector>
namespace tokext
{

class InputData;
class ExtractionParams;

class TokenUtility
{
public:
    TokenUtility(const InputData& idata);

    std::vector<Token> extractTokens(ExtractionParams& constraint);

    template <class T>
    std::vector<T> translateToken(const Token& token);

private:
    SuffixContext suffixContext_;
};

template <class T>
std::vector<T>  TokenUtility::translateToken(const Token& token)
{
    return utils::translateToken<T>(this->suffixContext_, token);
}

} /* tokext */


#endif /* end of include guard: TOKENUTILITY_HPP_CHV1ONWB */
