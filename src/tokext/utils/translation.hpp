
#ifndef TRANSLATION_HPP_H2S7XRTW
#define TRANSLATION_HPP_H2S7XRTW

#include "../Token.hpp"
#include "../SuffixContext.hpp"

#include <algorithm>

namespace tokext
{
    namespace utils
    {


template <class T>
T translateToken(
    const SuffixContext& context,
    const Token& token,
    bool safe = false)
{
    T ret;
    auto sufIdx = token.firstSufIdx();
    if(safe) {
        for(auto i = token.firstSufIdx(); i <= token.lastSufIdx(); ++i) {
            if(context.isSuspicious(i)) {
                sufIdx = i;
                break;
            }
        }
    }

    for(
        auto it = context.beginIBuffer() + context.suffix(sufIdx),
             itEnd = it + token.length();
        it != itEnd; ++it) {
        ret.push_back((*it) - context.shift());
    }
    return ret;
}

namespace
{

template<class T, class Compare>
struct OperatorAdaptor
{
    OperatorAdaptor(
            const SuffixContext& context,
            Compare comp, bool safe):
        context_(context), comp_(comp), safe_(safe)
    {}

    bool operator()(const Token& token, const T& t)
    {
        T trans = translateToken<T>(context_, token, safe_);
        return comp_(trans, t);
    }

    bool operator()(const T& t, const Token& token)
    {
        T trans = translateToken<T>(context_, token, safe_);
        return comp_(t, trans);
    }

private:
    const SuffixContext& context_;
    Compare comp_;
    bool safe_;
};

}

template<
    class ForwardIterator,
    class Str,
    class Compare = std::less<Str>>
ForwardIterator findToken(
    const SuffixContext& context,
    const Str& str_,
    ForwardIterator first_,
    ForwardIterator last_,
    bool safe = false,
    Compare comp = {})
{
    OperatorAdaptor<Str, Compare> adaptorComp(
            context, comp, safe);
    ForwardIterator it = std::lower_bound(first_, last_, str_, adaptorComp);
    if (it != last_ && !adaptorComp(str_, *it)) {
        return it;
    }
    return last_;
}

    } /* utils */
} /* tokext */

#endif /* end of include guard: TRANSLATION_HPP_H2S7XRTW */
