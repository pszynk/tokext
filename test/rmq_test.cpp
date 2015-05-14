
#include "test.hpp"
#include "tokext/testing/random_input.hpp"

#include "tokext/types.hpp"
#include "tokext/rmq/RmqWrapper.hpp"

#include <sstream>
#include <array>
#include <vector>
#include <iterator>

namespace
{
using namespace tokext;
using namespace tokext::testing;


const static idx_t max_val = std::numeric_limits<idx_t>::max() - 1;

template<typename T>
size_t test_rmq(size_t from, size_t to, std::vector<T>& data)
{
    if (from > to) {
        return test_rmq(to, from, data);
    }
    if (from == to) {
        return from;
    }
    auto bg = data.begin();
    auto ed = data.begin();
    std::advance(bg, from);
    std::advance(ed, to);
    return std::distance(data.begin(), std::min_element(bg, ed));
}

struct param_t
{
    param_t(size_t size, size_t gen,
            idx_t firstVal, idx_t lastVal):
        size_(size), gen_(gen),
        firstVal_(firstVal), lastVal_(lastVal)
    {};

    size_t size_;
    size_t gen_;
    idx_t firstVal_;
    idx_t lastVal_;

    friend std::ostream& operator<<(std::ostream& strm, const param_t& param)
    {
        return strm << "params:"<<
            " size="<<param.size_<<
            ", gen="<<param.gen_<<
            ", firstVal="<<param.firstVal_<<
            ", lastVal="<<param.lastVal_;
    };
};


TEST_CASE( "rmq random generation test", "[rmq][random][loop]")
{
    static std::mt19937 generator(getSeedValue());
    using namespace std;
    using namespace tokext::rmq;


    static const vector<param_t> params {
        {10   , 20 , 0, 20},
        {20   , 20 , 0, max_val},
        {100  , 40 , 0, 10},
        {500  , 50 , 0, 20},
        {1000 , 100, 0, max_val},
        {10000, 300, 0, 300},
    };

    for(const auto& p : params)
    {
        vector<idx_t> data = VectorRandomizer<idx_t>(
                NumberRandomizer<idx_t>(p.firstVal_, p.firstVal_),
                NumberRandomizer<size_t>(p.size_)).generate(generator);
        RmqWrapper rmqWrapper(data);

        ostringstream oss;
        oss << "rmq query | " << p;
        SECTION (oss.str(), "")
        {
            NumberRandomizer<idx_t> query(0, p.size_-1);
            for(size_t i = 0; i < p.gen_; ++i)
            {
                idx_t from = query.generate(generator);
                idx_t to = query.generate(generator);

                ostringstream oss;
                oss << "query nr. " << i;
                SECTION(oss.str(), "")
                {
                    INFO("("<<from<<", "<<to<<")");
                    auto wrappers = data[rmqWrapper.query(from, to)];
                    auto naive = data[test_rmq(from, to, data)];
                    REQUIRE( wrappers <= naive);
                }
            }
        }
    }
}

}
