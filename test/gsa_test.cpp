
#include "test.hpp"
#include "tokext/testing/random_input.hpp"

#include "tokext/types.hpp"
#include "tokext/SuffixContext.hpp"

#include <vector>
#include <sstream>
#include <ostream>
#include <algorithm>

namespace
{
using namespace tokext;
using namespace tokext::testing;

bool validate(const tokext::SuffixContext& scx)
{
    size_t data_size = scx.size();
    auto iend = scx.endIBuffer();
    for(size_t i = 1; i < data_size; ++i) {
        auto slr_first = scx.beginIBuffer() + scx.suffix(i-1);
        auto gtr_first = scx.beginIBuffer() + scx.suffix(i);

        if (! std::lexicographical_compare(slr_first, iend, gtr_first, iend)) {
                return false;
        }
    }
    return true;
}

struct param_t
{
    param_t(size_t repeat, size_t susp, size_t norm,
            size_t shortest, size_t longest,
            ichar_t firstChar, ichar_t lastChar):
        repeat_(repeat), susp_(susp), norm_(norm),
        shortest_(shortest), longest_(longest),
        firstChar_(firstChar), lastChar_(lastChar)
    {};

    size_t repeat_;
    size_t susp_;
    size_t norm_;
    size_t shortest_;
    size_t longest_;
    ichar_t firstChar_;
    ichar_t lastChar_;

    friend std::ostream& operator<<(std::ostream& strm, const param_t& param)
    {
        return strm << "params:"<<
            " reapeat="<<param.repeat_<<
            ", susp="<<param.susp_<<
            ", norm="<<param.norm_<<
            ", shortest_="<<param.shortest_<<
            ", longest_="<<param.longest_<<
            ", firstChar_="<<param.firstChar_<<
            ", lastChar_="<<param.lastChar_;
    };
};


TEST_CASE("gsa random generation test", "[gsa][random][loop]")
{
    using namespace std;
    static mt19937 generator(getSeedValue());

    static const vector<param_t> params {
        {100, 10  , 0   , 0   , 100 , 0  , 255},
        {100, 0   , 10  , 0   , 100 , 0  , 255},
        {10 , 100 , 100 , 0   , 100 , 37 , 111},
        {10 , 100 , 100 , 0   , 100 , 254, 255},
        {10 , 100 , 100 , 2000, 3000, 0  , 255},
        {2  , 1000, 1000, 1000, 3000, 0  , 255},
    };

    for(const auto& p: params)
    {
        InputDataRandomizer inputGen(
                VectorRandomizer<ichar_t>(
                    NumberRandomizer<ichar_t>(p.firstChar_, p.lastChar_),
                    NumberRandomizer<size_t>(p.shortest_, p.longest_)));

        ostringstream oss;
        oss << "gsa random validation | " << p;

        SECTION(oss.str(), "")
        {
            for(size_t i = 0; i < p.repeat_; ++i)
            {
                ostringstream oss;
                oss << "gsa repetition no. " << i;
                SECTION(oss.str(), "")
                {
                    tokext::SuffixContext scx(
                            inputGen.generate(generator, p.susp_, p.norm_));
                    REQUIRE(validate(scx) == true);
                }
            }
        }
    }
}

}
