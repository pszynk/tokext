
#ifndef RANDOMIZATION_HPP_7VTIQNUQ
#define RANDOMIZATION_HPP_7VTIQNUQ

#include "tokext/types.hpp"
#include "tokext/InputData.hpp"

#include <vector>
#include <random>
#include <cassert>
//#include <ostream>


namespace tokext
{
    namespace testing
    {


template<typename E>
class NumberRandomizer
{
public:
    NumberRandomizer(E lower, E upper):
        distribution_(lower, upper)
    {};
    NumberRandomizer(E constant):
        noRandom_(true), constant_(constant)
    {};

    template<class URGN>
    E generate(URGN& g)
    {
        return (noRandom_ ? constant_ : distribution_(g));
    }
private:
    bool noRandom_ = false;
    E constant_ = 0;
    std::uniform_int_distribution<E> distribution_;

};

template<typename E>
class VectorRandomizer {
public:
    VectorRandomizer(
            NumberRandomizer<E> valRandomizer,
            NumberRandomizer<size_t> lenRandomizer):
        valRandomizer_(valRandomizer), lenRandomizer_(lenRandomizer)
    {};

    template<class URGN>
    std::vector<E> generate(URGN& g)
    {
        std::vector<E> vec;
        size_t len = lenRandomizer_.generate(g);
        for(size_t i = 0; i != len; ++i) {
            vec.push_back(valRandomizer_.generate(g));
        }
        return vec;
    }

private:
    NumberRandomizer<E> valRandomizer_;
    NumberRandomizer<size_t> lenRandomizer_;
};

class TokensRandomizer
{
    // TODO:
    // InputDataRandomizer +
    // inject tokens with specified occurrence
    // generate tokens such:
    //  some tokens should be parts of other tokens
    //  e.g.
    //      ###asd######     <- token A
    //      ######asd######  <- token B
    //  so
    //      asd              <- roken C such occ(C) >= occ(A) + occ(B)
    //                                      [occ - occurrence]
    // (25/10/14 17:42:38 - pawel)
public:
    TokensRandomizer ();
    virtual ~TokensRandomizer ();

private:

};


class InputDataRandomizer
{
public:

    InputDataRandomizer(VectorRandomizer<tokext::ichar_t> vectorRandomizer):
        vectorRandomizer_(vectorRandomizer)
    {};

    template<class URGN>
    tokext::InputData generate(URGN& g, size_t suspicious, size_t normal)
    {
        tokext::InputData data;
        for(size_t i = 0; i < suspicious; ++i) {
            //std::cout << "susp nr " << i << std::endl;
            data.putSuspicious(vectorRandomizer_.generate(g));
        }
        for(size_t i = 0; i < normal; ++i) {
            //std::cout << "norm nr " << i << "|"<< normal<< std::endl;
            data.putNormal(vectorRandomizer_.generate(g));
        }
        return data;
    }
private:
    VectorRandomizer<tokext::ichar_t> vectorRandomizer_;
};

//TODO: randomizer with token data [(20/01/15 16:09:09) - pawel]
class InputDataTokenizedRandomizer
{
public:
    InputDataTokenizedRandomizer ();
    virtual ~InputDataTokenizedRandomizer ();

private:

};

    } /* testing */
} /* tokext */
#endif /* end of include guard: RANDOMIZATION_H_7VTIQNUQ */
