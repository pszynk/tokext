
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <vector>

#define TX_XCHAR_BITS (16)
#define TX_XCHAR_MAX ((1 << TX_XCHAR_BITS) - 1)


namespace tokext {

/**
 * @brief Input character type for input and output
 */
typedef uint8_t ichar_t;

/**
 * @brief eXtended character type used in module,
 * for encoding string for suffix array creation.
 */
typedef uint16_t xchar_t;

/**
 * @brief unsigned integer type for indexing normal size arrays used in
 * module
 */
typedef uint32_t idx_t;


/**
 * @brief unsigned integer type for indexing arrays that don't exceed max flow
 * count
 */
typedef uint16_t flow_idx_t;

// TODO: wszystko poniżej albo na klasy albo wywalić (09/08/14 20:27:41 - pawel)
//typedef std::vector<rchar_t> tx_I_string_t;
//typedef std::vector<xchar_t> tx_E_string_t;

//typedef std::vector<std::vector<rchar_t>> tx_flows_t;
//typedef uint16_t tx_flowidx_t;
//typedef std::vector<tx_flowidx_t> std::vector<flowidx_t>;

//typedef std::vector<idx_t> tx_suf_vec_t;
//typedef std::vector<idx_t> tx_idx_vec_t;

}


#endif /* end of include guard: TX_TYPES_H */
