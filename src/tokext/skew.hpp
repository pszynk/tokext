/**
 *      @file  skew.h
 *     @brief
 *
 * Detailed description starts here.
 *
 *    @author  Paweł Szynkiewicz (PSz), pszynk@gmail.com
 *
 *  @internal
 *    Created  22.05.2014 15:21:27
 *   Revision  $REVISION$
 *   Compiler  g++
 *    Company  Politechnika Warszawska
 *  Copyright  Copyright (c) 2014, Paweł Szynkiewicz
 *
 *=====================================================================================
 */

#ifndef SKEW_H
#define SKEW_H

#include "types.hpp"

#include <vector>

namespace tokext {

    namespace skew
    {

// rozmiar litery dla alg. skew, bezpiecznie większy bo
// kodowanie trójek jest rekurencyjne
typedef uint32_t char_t;
typedef std::vector<char_t> vec_t;

/*
 *template <typename ItT>
 *std::vector<idx_t> skewSuffixArray(
 *        ItT bbegin,
 *        ItT bend,
 *        idx_t alphabetSize)
 *{
 *    return skewSuffixArray()
 *}
 */

std::vector<idx_t> skewSuffixArray(
        vec_t buffer,
        idx_t alphabetSize);

    } /* skew */
} /* tokext */

#endif /* end of include guard: SKEW_H */
