
#include "test.hpp"
#include "tokext/testing/random_input.hpp"

#include "tokext/types.hpp"
#include "tokext/SuffixContext.hpp"
#include "tokext/ExtractionParams.hpp"
#include "tokext/utils/extraction.hpp"
#include "tokext/utils/translation.hpp"

#include <vector>
#include <sstream>
#include <ostream>
#include <algorithm>

namespace
{

using namespace tokext;
using namespace tokext::testing;

bool uniqueTokensExtracted(const SuffixContext& scx, ExtractionParams& extParams)
{
    std::cerr << "Starting Exctaction, data size:" << scx.size() << std::endl;
    auto tokens = utils::extractTokens(scx, extParams);
    if (tokens.empty()) {
        //WARN("no tokens found");
        return true;
    }
    //WARN("found tokens "<<tokens.size());

    using tdata_t = std::vector<ichar_t>;
    tdata_t prev;
    size_t counter = 0;
    for(auto& t : tokens) {
        tdata_t next = utils::translateToken<tdata_t>(scx, t);
        if (! std::lexicographical_compare(
                    prev.begin(), prev.end(), next.begin(), next.end())) {
                return false;
        }
        prev = std::move(next);
    }
    return true;
}

struct param_t
{
    param_t(size_t repeat, size_t susp,
            size_t shortest, size_t longest,
            ichar_t firstChar, ichar_t lastChar,
            idx_t minTokLen, idx_t maxTokLen,
            idx_t minTokOcc, idx_t maxTokOcc):
        repeat_(repeat), susp_(susp),
        shortest_(shortest), longest_(longest),
        firstChar_(firstChar), lastChar_(lastChar),
        minTokLen_(minTokLen), maxTokLen_(maxTokLen),
        minTokOcc_(minTokOcc), maxTokOcc_(maxTokOcc)
    {};

    size_t repeat_;
    size_t susp_;
    size_t shortest_;
    size_t longest_;
    ichar_t firstChar_;
    ichar_t lastChar_;
    idx_t minTokLen_;
    idx_t maxTokLen_;
    idx_t minTokOcc_;
    idx_t maxTokOcc_;

    friend std::ostream& operator<<(std::ostream& strm, const param_t& param)
    {
        return strm << "params:"<<
            " reapeat="<<param.repeat_<<
            ", susp="<<param.susp_<<
            ", shortest_="<<param.shortest_<<
            ", longest_="<<param.longest_<<
            ", firstChar_="<<param.firstChar_<<
            ", lastChar_="<<param.lastChar_<<
            ", minTokLen_="<<param.minTokLen_<<
            ", maxTokLen_="<<param.maxTokLen_<<
            ", minTokOcc_="<<param.minTokOcc_<<
            ", maxTokOcc_="<<param.maxTokOcc_;
    };
};


TEST_CASE("token uniqueness random generation test", "[token][unique][random][loop]")
{
    using namespace std;

    static mt19937 generator(getSeedValue());

    static const vector<param_t> params {
        {10 , 10   , 0    , 100  , 0   , 5   , 3 , 3 , 2  , 2}  ,
        {10 , 100  , 0    , 100  , 37  , 111 , 3 , 3 , 2  , 2}  ,
        {5  , 100  , 0    , 100  , 250 , 255 , 3 , 3 , 2  , 2}  ,
        {5  , 100  , 100  , 100  , 100 , 150 , 3 , 3 , 2  , 2}  ,
        {5  , 100  , 1000 , 2000 , 0   , 255 , 3 , 3 , 4  , 10} ,
        {2  , 1000 , 1000 , 2000 , 0   , 255 , 3 , 3 , 10 , 10} ,
    };

    //1. wygenerować losowe dane (InputData)
    for(const auto& p: params)
    {
        InputDataRandomizer inputGen(
                VectorRandomizer<ichar_t>(
                    NumberRandomizer<ichar_t>(p.firstChar_, p.lastChar_),
                    NumberRandomizer<size_t>(p.shortest_, p.longest_)));

        NumberRandomizer<idx_t> lenConstGen(p.minTokLen_, p.maxTokLen_);
        NumberRandomizer<idx_t> occConstGen(p.minTokOcc_, p.maxTokOcc_);

        ostringstream oss;
        oss << "token unique check | " << p;

        SECTION(oss.str(), "")
        {
    //2. ekstrakcja tokenów
            for(size_t i = 0; i < p.repeat_; ++i)
            {
                ostringstream oss;
                oss << "unique repetition no. " << i << " " << p;
                SECTION(oss.str(), "")
                {
                    tokext::SuffixContext scx(
                            inputGen.generate(generator, p.susp_, 0));
                    ExtractionParams extParams(
                            lenConstGen.generate(generator),
                            occConstGen.generate(generator));
                    REQUIRE(uniqueTokensExtracted(scx, extParams) == true);
                }
            }
        }
    }

}
}
