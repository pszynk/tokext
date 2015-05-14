#ifndef RMQWRAPPER_HPP_PORFJUGV
#define RMQWRAPPER_HPP_PORFJUGV

#include <cstdlib>
#include <stack>
#include <ostream>

#include "../types.hpp"

namespace tokext
{
    namespace rmq
    {

class RmqWrapper
{
private:
    static const size_t NULL_NODE;
    // TODO: make generic, every sequential data structure containing type
    // that can be compared (04/10/14 20:12:18 - pawel)
    typedef std::vector<idx_t> vec_t;

    typedef std::vector<size_t> idx_vector;
    typedef std::vector<idx_vector> sparse_table; //TODO lepsza reprezentacja?
    typedef std::vector<sparse_table> sparse_table_map;
    typedef std::vector<bool> binary_vector;

public:
    RmqWrapper(const vec_t &vec);

    size_t query(size_t i, size_t j) const;

private:
    size_t blockSize_;
    size_t blockCount_;
    size_t intsInBlock_;

    /**
     * @brief euler path on Cartesian Tree build on given vector to do RMQ on
     */
    idx_vector eulers_;

    /**
     * @brief when (idx) this index occurred first in Euler path
     */
    idx_vector firsts_;

    /**
     * @brief levels of Euler path
     */
    idx_vector levels_;

    /**
     * @brief minimum (level) in each block
     */
    idx_vector blocksMin_;

    /**
     * @brief index (in Euler path) of minimum (level) in each block
     */
    idx_vector blocksMinIdx_;

    /**
     * @brief lookup matrix computed from vector of mins of blocks of
     * Euler path
     *
     * Matrix [blockCount_] x [lg(blockSize_)] - not really a matrix
     * has max lg columns. M[i][j] is min of range [i, i + 2^j], of
     * V it was computed from. Range never exceeds size of V.
     */
    sparse_table globals_;

    /**
     * @brief lookup map of matrices computed from levels of euler path
     *
     * Map {block [binary rep.]} -> {matrix [sparse_table]} contains lookup
     * matrices for each binary rep. of block.
     */
    sparse_table_map locals_;

    /**
     * @brief maps block_id -> block_hash (binary value corresponding to this
     * block levels)
     */
    idx_vector blockMapper_;

    size_t getBlockId(size_t idx) const;
    size_t getBlockOffset(size_t idx) const;

    size_t sparseRmq(size_t from, size_t to, size_t offset, const idx_vector& values, const sparse_table& sparse) const;

    size_t query_impl(size_t i, size_t j) const;

    friend std::ostream& operator<<(std::ostream&, const RmqWrapper&);
};

std::ostream& operator<<(std::ostream&, const RmqWrapper&);

    } /* rmq */
} /* tokext */

#endif /* end of include guard: RMQWRAPPER_HPP_PORFJUGV */
