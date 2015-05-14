
#include "extraction_test_tools.hpp"

#include "tokext/types.hpp"
#include "tokext/InputData.hpp"
#include "tokext/SuffixContext.hpp"
#include "tokext/ExtractionParams.hpp"
#include "tokext/utils/extraction.hpp"
#include "tokext/utils/translation.hpp"

#include <map>

namespace ttool
{
    namespace
    {

using namespace tokext;

struct TokenMeta
{
    size_t lastOccurred;
    Occurrence occurrence;
};

using buffer_t_ = std::vector<tokext::ichar_t>;

using index_t_ =
    std::map<buffer_t_, TokenMeta>;

void add_(index_t_& index,
        bool modelIsSusp,
        bool appliedIsSusp,
        idx_t idx, idx_t len,
        buffer_t_::const_iterator start,
        buffer_t_::const_iterator finish)
{

    if ((finish - start) < len) {
        return;
    }

    auto& meta = index[buffer_t_(start, finish)];
    if (meta.lastOccurred >= idx) {
        return;
    }
    if (meta.lastOccurred == 0) {
        meta.occurrence.incIn(modelIsSusp);
    }

    meta.lastOccurred = idx;
    meta.occurrence.incIn(appliedIsSusp);
    add_(index, modelIsSusp, appliedIsSusp, idx, len, start+1, finish);
}

} /* unnamed namespace */

void applyTo_(index_t_& index,
        size_t idx,
        size_t minLen,
        bool modelIsSusp,
        const buffer_t_& model,
        bool appliedIsSusp,
        const buffer_t_& applied)
{
    using namespace std;
    using namespace tokext;

    if (applied.empty()) {
        return;
    }

    vector<idx_t> curr(applied.size());
    vector<idx_t> prev(applied.size());

    for(idx_t i = 0; i < model.size(); ++i) {
        size_t added = minLen;
        for(idx_t j = 0; j < applied.size(); ++j) {
            if (model[i] != applied[j]) {
                curr[j] = 0;
            }
            else {
                if (i == 0 || j == 0) {
                    curr[j] = 1;
                }
                else {
                    curr[j] = 1 + prev[j-1];

                    if (curr[j] >= added) {
                        added = curr[j];
                        add_(index,
                                modelIsSusp,
                                appliedIsSusp,
                                idx,
                                minLen,
                                applied.begin() + (j + 1 - curr[j]),
                                applied.begin() + (j + 1));
                    }
                }
            }
        }
        curr.swap(prev);
    }
}

std::vector<TestToken> naiveTokenExtraction(
        const tokext::InputData& idata,
        tokext::idx_t minLen, tokext::Occurrence minOcc)
{
    using namespace std;
    using namespace tokext;

    index_t_ index;

    // starts from 1
    idx_t idx = 0;
    for(auto itOut = idata.beginSuspicious();
            itOut != idata.endSuspicious(); ++itOut) {
        ++idx;
        if (itOut->empty()) {
            continue;
        }

        for(auto itSusp = itOut + 1; itSusp != idata.endSuspicious(); ++itSusp) {
            applyTo_(index,  idx, minLen, true, (*itOut), true, (*itSusp));
        }
        for(auto itNorm = idata.beginNormal(); itNorm != idata.endNormal(); ++itNorm) {
            applyTo_(index, idx, minLen, true, (*itOut), false, (*itNorm));
        }
    }

    for(auto itOut = idata.beginNormal();
            itOut != idata.endNormal(); ++itOut) {
        ++idx;
        if (itOut->empty()) {
            continue;
        }
        for(auto itNorm = itOut + 1; itNorm != idata.endNormal(); ++itNorm) {
            applyTo_(index, idx, minLen, false, (*itOut), false, (*itNorm));
        }
    }

    vector<TestToken> rtrn;
    for (auto item : index) {
        //cout << item.second.occurrence << " : " << item.first << endl;
        if (item.second.occurrence >= minOcc) {
            rtrn.emplace_back(item.second.occurrence, item.first);
        }
    }

    return rtrn;
}

std::vector <TestToken> smartTokenExtraction(
        const tokext::SuffixContext& scx, tokext::ExtractionParams& extParams)
{
    using namespace std;
    using namespace tokext;

    auto tokens = utils::extractTokens(scx, extParams);
    if (tokens.empty()) {
        //WARN("no tokens found");
    }
    //WARN("found tokens "<<tokens.size());

    vector<TestToken> rtrn;
    for(auto t : tokens) {
        rtrn.emplace_back(t.occurrence(), utils::translateToken<buffer_t_>(scx, t));
    }
    return rtrn;
}

} /* ttool */
