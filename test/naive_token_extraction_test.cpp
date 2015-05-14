
#include "test.hpp"

#include "tokext/testing/random_input.hpp"
#include "ttool/extraction_test_tools.hpp"

#include "tokext/types.hpp"
#include "tokext/Occurrence.hpp"
#include "tokext/InputData.hpp"
#include "tokext/SuffixContext.hpp"
#include "tokext/ExtractionParams.hpp"
#include "tokext/utils/extraction.hpp"
#include "tokext/utils/translation.hpp"

#include "prettyprint/prettyprint.h"

#include <ostream>
#include <vector>
#include <map>

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
            ", norm_="<<param.susp_<<
            ", shortest_="<<param.shortest_<<
            ", longest_="<<param.longest_<<
            ", firstChar_="<<param.firstChar_<<
            ", lastChar_="<<param.lastChar_<<
            ", minLen_="<<param.minLen_<<
            ", minOcc_="<<param.minOcc_;
    };
};

TEST_CASE("naive by hand token extraction test", "[token][naive][byhand]")
{
    static mt19937 generator(getSeedValue());
    SECTION("abcdef, xabcdefz, bcd")
    {
        InputDataRandomizer inputGen(
                VectorRandomizer<ichar_t>(
                    NumberRandomizer<ichar_t>('a', 'c'),
                    NumberRandomizer<size_t>(4, 10)));
        auto idata = inputGen.generate(generator,4, 2);
        //auto idata = makeInputData<vector<string>>({"abcdef", "xabcdefz", "bcd"}, {});
        SuffixContext scx(idata);
        ExtractionParams params(2, 2);
        auto tokens = utils::extractTokens(scx, params);

        auto smart = ttool::smartTokenExtraction(scx, params);
        auto naive = ttool::naiveTokenExtraction(idata, params.reqLength(), params.reqOccurrence());
        //cout << "IDATA" << endl << idata << endl;
        //cout << "NAIVE" << endl << naive << endl;
        //cout << "SMART" << endl << smart << endl;
        REQUIRE(naive.size() == smart.size());
        REQUIRE(equal(smart.begin(), smart.end(), naive.begin()));

    }
}

TEST_CASE("naive full token extraction test", "[token][naive][random][loop]")
{
    static mt19937 generator(getSeedValue());

    static vector<param_t> params {
        {10 , 30  , 0   , 100 , 150 , 0   , 255 , 2 , {2 , 0}} ,
        {10 , 0   , 30  , 100 , 150 , 0   , 255 , 2 , {2 , 0}} ,
        {10 , 30  , 0   , 100 , 150 , 0   , 255 , 2 , {0 , 2}} ,
        {10 , 0   , 30  , 100 , 150 , 0   , 255 , 2 , {0 , 2}} ,
        {10 , 30  , 30  , 100 , 150 , 0   , 255 , 2 , {1 , 1}} ,
        {10 , 30  , 30  , 100 , 150 , 0   , 255 , 2 , {1 , 1}} ,
        {5  , 100 , 150 , 50  , 80  , 0   , 50  , 3 , {3 , 0}} ,
        {5  , 50  , 20  , 150 , 200 , 200 , 55  , 3 , {3 , 0}} ,
    };

    //1. wygenerować losowe dane (InputData)
    for(const auto& p: params)
    {
        InputDataRandomizer inputGen(
                VectorRandomizer<ichar_t>(
                    NumberRandomizer<ichar_t>(p.firstChar_, p.lastChar_),
                    NumberRandomizer<size_t>(p.shortest_, p.longest_)));

        ostringstream oss;
        oss << "token extraction with naive check | " << p;

        SECTION(oss.str(), "")
        {
            //2. ekstrakcja tokenów
            for(size_t i = 0; i < p.repeat_; ++i)
            {
                ostringstream oss;
                oss << "naive repetition no. " << i << " " << p;
                SECTION(oss.str(), "")
                {
                    auto idata = inputGen.generate(generator, p.susp_, p.norm_);
                    tokext::SuffixContext scx(idata);
                    ExtractionParams extParams(p.minLen_, p.minOcc_);

                    auto smart = ttool::smartTokenExtraction(scx, extParams);
                    auto naive = ttool::naiveTokenExtraction(idata, p.minLen_, p.minOcc_);
                    //cout << "NAIVE" << endl << naive << endl;
                    //cout << "SMART" << endl << smart << endl;
                    REQUIRE(naive.size() == smart.size());
                    REQUIRE(equal(smart.begin(), smart.end(), naive.begin()));
                }
            }
        }
    }
}
}
