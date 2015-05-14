
#include "test.hpp"

#include "tokext/testing/random_input.hpp"

#include "tokext/types.hpp"
#include "tokext/Occurrence.hpp"
#include "tokext/InputData.hpp"
#include "tokext/Token.hpp"
#include "tokext/SuffixContext.hpp"
#include "tokext/ExtractionParams.hpp"
#include "tokext/utils/extraction.hpp"


#include <algorithm>
#include <ostream>

namespace
{

using namespace tokext;
using namespace tokext::testing;
using namespace std;


struct param_t
{
    param_t(size_t repeat,
            size_t susp, size_t norm,
            size_t shortest, size_t longest,
            ichar_t firstChar, ichar_t lastChar,
            idx_t minTokLen,
            tokext::Occurrence minOcc):
        repeat_(repeat),
        susp_(susp), norm_(norm),
        shortest_(shortest), longest_(longest),
        firstChar_(firstChar), lastChar_(lastChar),
        minLen_(minTokLen),
        minOcc_(minOcc)
    {};

    size_t repeat_;
    size_t susp_;
    size_t norm_;
    size_t shortest_;
    size_t longest_;
    ichar_t firstChar_;
    ichar_t lastChar_;
    idx_t minLen_;
    tokext::Occurrence minOcc_;

    friend std::ostream& operator<<(std::ostream& strm, const param_t& param)
    {
        return strm << "params:"<<
            " reapeat="<<param.repeat_<<
            ", susp_="<<param.susp_<<
            ", norm_="<<param.norm_<<
            ", shortest_="<<param.shortest_<<
            ", longest_="<<param.longest_<<
            ", firstChar_="<<param.firstChar_<<
            ", lastChar_="<<param.lastChar_<<
            ", minLen_="<<param.minLen_<<
            ", minOcc_="<<param.minOcc_;
    };
};

TEST_CASE("location of extracted tokens test", "[token][location][random][loop]")
{
    static mt19937 generator(getSeedValue());

    static vector<param_t> params {
        {5 , 100 , 100 , 10   , 20   , 'a' , 'e' , 2 , {2 , 0}} ,
        {5 , 300 , 0   , 10   , 20   , 'a' , 'z' , 2 , {2 , 0}} ,
        {5 , 0   , 300 , 1000 , 1500 , 0   , 255 , 2 , {0 , 2}} ,
        {5 , 300 , 0   , 1000 , 1500 , 0   , 255 , 2 , {2 , 0}} ,
        {5 , 0   , 300 , 1000 , 1500 , 0   , 255 , 2 , {0 , 2}} ,
        {5 , 300 , 300 , 1000 , 1500 , 0   , 255 , 2 , {1 , 1}} ,
        {5 , 300 , 300 , 1000 , 1500 , 0   , 255 , 2 , {1 , 1}} ,
        {2 , 100 , 150 , 500  , 800  , 0   , 50  , 3 , {3 , 0}} ,
        {2 , 500 , 200 , 150  , 200  , 5   , 55  , 3 , {3 , 0}} ,
    };

    //1. wygenerować losowe dane (InputData)
    for(const auto& p: params)
    {
        InputDataRandomizer inputGen(
                VectorRandomizer<ichar_t>(
                    NumberRandomizer<ichar_t>(p.firstChar_, p.lastChar_),
                    NumberRandomizer<size_t>(p.shortest_, p.longest_)));

        ostringstream oss;
        oss << "token extraction with location of tokens check | " << p;

        SECTION(oss.str(), "")
        {
            //2. ekstrakcja tokenów
            for(size_t i = 0; i < p.repeat_; ++i)
            {
                ostringstream oss;
                oss << "location check repetition no. " << i << " " << p;
                SECTION(oss.str(), "")
                {
                    auto idata = inputGen.generate(generator, p.susp_, p.norm_);
                    tokext::SuffixContext scx(idata);
                    ExtractionParams extParams(p.minLen_, p.minOcc_);

                    auto tokens = utils::extractTokens(scx, extParams);
                    if (tokens.empty()) {
                        WARN("no tokens found");
                        continue;
                    }
                    //WARN("found tokens "<<tokens.size());

                    for(auto& t : tokens)
                    {
                        INFO(t);
                        // token cant start on first N suffixes (where N
                        // is number of flows)
                        REQUIRE(t.firstSufIdx() >= scx.totalFlowCount());
                        // first suffix always not grater that last
                        REQUIRE(t.firstSufIdx() <= t.lastSufIdx());
                        // lcp[first] < len
                        REQUIRE(scx.lcp(t.firstSufIdx()) < t.length());
                        // lcp[last+1] < len or doesn't exists
                        if(scx.size() != t.lastSufIdx() + 1) {
                            REQUIRE(scx.lcp(t.lastSufIdx() + 1) < t.length());
                        }
                        // lcp[first+1:last] >= len
                        for(idx_t i = t.firstSufIdx() + 1; i <= t.lastSufIdx();
                                ++i) {
                            REQUIRE(scx.lcp(i) >= t.length());
                        }
                    }

                }
            }
        }
    }
}
}
