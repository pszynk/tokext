
#include "RmqWrapper.hpp"

#include <limits>
#include <math.h>
#include <assert.h>

#include <prettyprint/prettyprint.h>

namespace tokext
{
    namespace rmq
    {
        namespace
        {
//{{{1
using vec_t = std::vector<idx_t>;
using idx_vector = std::vector<size_t>;
using sparse_table = std::vector<idx_vector>;
using binary_vector = std::vector<bool>;
using sparse_table_map = std::vector<sparse_table>;

const size_t NULL_NODE = std::numeric_limits<size_t>::max();

size_t computeParents(const vec_t &values,
        idx_vector &parents)
{
    std::stack<size_t> st;
    parents.resize(values.size());
    size_t head = NULL_NODE; //TODO co jak uninitialized

    //we start with an empty stack
    //at step i we insert A[i] in the stack
    for (size_t i = 0; i < values.size(); ++i) {

        size_t last;
        bool popped = false;
        while ((!st.empty()) && (values[st.top()] > values[i])) {
            last = st.top();
            popped = true;
            st.pop();
        }
        if (!st.empty()) {
            parents[i] = st.top();
        } else {
            head = i;
        }
        if (popped) {
            parents[last] = i;
        }
        st.push(i);
    }
    parents[head] = NULL_NODE;
    return head;
}

void computeSons(
        size_t root,
        const idx_vector &parents,
        idx_vector &lefts, idx_vector &rights)
{
    lefts.resize(parents.size(), NULL_NODE);
    rights.resize(parents.size(), NULL_NODE);

    for(size_t i = 0; i < parents.size(); ++i) {
        if (i == root) continue;
        size_t parent = parents[i];
        if (parent <= i) {
            rights[parent] = i;
        } else {
            lefts[parent] = i;
        }
    }
}

void computeLca(size_t root,
        idx_vector &lefts,
        idx_vector &rights,
        idx_vector &eulers,
        idx_vector &levels,
        idx_vector &firsts)
{
    eulers.reserve(2 * lefts.size() - 1);
    levels.reserve(2 * lefts.size() - 1);
    firsts.resize(lefts.size(), NULL_NODE);

    std::stack<size_t> st;
    st.push(root);
    while (!st.empty()) {
        size_t node = st.top();
        eulers.push_back(node);
        if(firsts[node] == NULL_NODE) {
            firsts[node] = eulers.size() - 1;
        }
        levels.push_back(st.size()-1);

        size_t left, right;

        left = lefts[node];
        if (left != NULL_NODE) {
            st.push(left);
            lefts[node] = NULL_NODE;
            continue;
        }

        right = rights[node];
        if (right != NULL_NODE) {
            st.push(right);
            rights[node] = NULL_NODE;
            continue;
        }

        st.pop();
    }
}
void computeRestricted(
        const idx_vector &levels,
        binary_vector &binary)
{
    binary.resize(levels.size(), 0);
    for (size_t i = 1; i < binary.size(); ++i) {
        if (levels[i] > levels[i-1]) {
            binary[i] = true;
        }
    }
}

void computeBlocks(
        size_t blockCount,
        size_t blockSize,
        const idx_vector &levels,
        idx_vector &minBlock,
        idx_vector &lookupBlock)
{
    minBlock.resize(blockCount);
    lookupBlock.resize(blockCount);
    for(size_t b = 0; b < blockCount; ++b) {
        size_t minLvlVal = NULL_NODE;
        size_t minLvlIdx = 0;
        for(size_t ib = 0; ib < blockSize; ++ib) {
            size_t idx = blockSize * b + ib;
            if (idx >= levels.size()) {
                break;
            }
            if (minLvlVal > levels[idx]) {
                minLvlVal = levels[idx];
                minLvlIdx = idx;
            }
        }
        minBlock[b] = minLvlVal;
        lookupBlock[b] = minLvlIdx;
    }
}

void preprocess(
        const size_t * const values,
        size_t len,
        sparse_table &lookups)
{
    lookups.resize(len);
    for(size_t i = 0; i < len; ++i) {
        lookups[i].resize(ceil(log(len - i + 1) / log(2)));
    }

    for(size_t i = 0; i < len; ++i) {
        lookups[i][0] = i; //TODO można wsadzic w petle wyzej
    }

    for (size_t j = 1; 1u << j <= len; j++) {
        for (size_t i = 0; i + (1 << j) - 1 < len; i++) {
            size_t o1 = lookups[i][j - 1];
            size_t o2 = lookups[i + (1 << (j - 1))][j - 1];
            if (values[o1] < values[o2]) {
                lookups[i][j] = o1;
            }
            else {
                  lookups[i][j] = o2;
            }
        }
    }
}

void preprocess(
        const idx_vector &values,
        sparse_table &lookups)
{
    preprocess(&values[0], values.size(), lookups);
}

size_t blockHash(const binary_vector &binary, size_t blockSize, size_t blockIdx)
{
    size_t id = 0;
    size_t blockStart = blockIdx * blockSize;
    for(size_t i = blockStart;
            (i < (blockStart + blockSize)) && (i < binary.size());
            ++i) {
        id *= 2;
        id += binary[i];
    }
    return id;
}

void computeLocals(
        size_t blockCount,
        size_t blockSize,
        const idx_vector &values,
        const binary_vector &binary,
        idx_vector &mapper,
        sparse_table_map &locals)
{
    // ile jest podprzedziałów w przedziale [0 blockSize-1] //TODO moze to za malo?
    size_t intCount = ceil((log(blockSize) / log(2)) * blockSize);
    //size_t intCount = (this->block_size + 1) * this->block_size / 2;

    // ilość binarnych ciągów o długości blockSize
    size_t locSize = pow(2, blockSize)
        * intCount;

    locals.resize(locSize);

    mapper.reserve(blockCount);

    size_t maxHsh = 0;
    for(size_t block = 0; block < blockCount; ++block) {
        size_t hsh = blockHash(binary, blockSize, block);
        mapper.push_back(hsh);

        if (!locals[hsh].empty()) {
            continue;
        }

        if (maxHsh < hsh) {
            maxHsh = hsh;
        }

        preprocess(&values[block * blockSize], blockSize, locals[hsh]);

    }
    locals.resize(maxHsh + 1);
    locals.shrink_to_fit();
}

void computeGlobals(
        const idx_vector &values,
        sparse_table &globals)
{
    //preprocess(&values[0], values.size(), globals);
    preprocess(values, globals);
}

} /* __local__ */
//1}}}

const size_t RmqWrapper::NULL_NODE = std::numeric_limits<size_t>::max();

RmqWrapper::RmqWrapper(const vec_t &values)
{
    // init sizes
    size_t size = 2 * values.size() - 1;
    this->blockSize_ = ceil((log(size) / log(2)) / (double) 2);
    this->blockCount_ = ceil((double) size / (double) this->blockSize_);

    // 1) compute tree
    //  1.1) values -> parents and root
    size_t root;

    {
        idx_vector lefts, rights;
        {

            idx_vector parents;
            root = computeParents(values, parents);

            //  1.2) parents and root -> left and right sons
            computeSons(root, parents, lefts, rights);
        }

        // 2) lca
        //  2.1) sons -> Euler path, levels, firsts idx in euler
        computeLca(root, lefts, rights, this->eulers_, this->levels_, this->firsts_);
    }

    //  2.2) levels -> binary rep. for restricted
    binary_vector binary;
    computeRestricted(this->levels_, binary);

    // 3) RMQ
    //  3.1) levels -> BLOCKS min in block, block lookup
    {
        computeBlocks(this->blockCount_, this->blockSize_, this->levels_,
                this->blocksMin_, this->blocksMinIdx_);

        //  3.2) min blocks -> globals
        computeGlobals(this->blocksMin_, this->globals_);
    }

    //  3.3) levels, binary -> map of locale intervals
    computeLocals(this->blockCount_, this->blockSize_, this->levels_,
            binary, this->blockMapper_, this->locals_);
}

size_t RmqWrapper::sparseRmq(size_t from, size_t to, size_t offset, const idx_vector& values, const sparse_table& sparse) const
{
    if (from == to) {
        return sparse[from][0] + offset; //TODO odkomentowalem
    }
    assert(from <= to);
    size_t k = (size_t) (log(to - from + 1)/log(2));
    size_t l = (size_t) to - (1 << k) + 1;

    if (values[sparse[from][k] + offset] <= values[sparse[l][k] + offset]) {
        return sparse[from][k] + offset;
    }
    return sparse[l][k] + offset;
}

size_t RmqWrapper::query(size_t from, size_t to) const
{
    if (from > to) {
        return query(to, from);
    }
    size_t ret = this->query_impl(from, to);
    assert(ret >= from);
    assert(ret <= to);
    return ret;
}

size_t RmqWrapper::query_impl(size_t fromIdx, size_t toIdx) const
{
    assert(toIdx < firsts_.size());
    size_t from = this->firsts_[fromIdx];
    size_t to = this->firsts_[toIdx];

    if (from > to) {
        return this->query_impl(toIdx, fromIdx);
    }
    else if (from == to) {
        return fromIdx;
    }

    size_t iBlockId = this->getBlockId(from);
    size_t iBlockOffset = this->getBlockOffset(from);

    size_t jBlockId = this->getBlockId(to);
    size_t jBlockOffset = this->getBlockOffset(to);

    // its the same block so query this block local
    if (iBlockId == jBlockId) {
        size_t idx = this->sparseRmq(iBlockOffset, jBlockOffset, iBlockId * this->blockSize_, this->levels_,
                this->locals_[this->blockMapper_[iBlockId]]);
        return this->eulers_[idx];
    }

    size_t fromBlockMinIdx = this->sparseRmq(iBlockOffset, this->blockSize_ - 1,
            iBlockId * this->blockSize_, this->levels_, this->locals_[this->blockMapper_[iBlockId]]);

    size_t toBlockMinIdx = this->sparseRmq(0, jBlockOffset,
            jBlockId * this->blockSize_, this->levels_, this->locals_[this->blockMapper_[jBlockId]]);


    size_t fromBlockMin =  fromBlockMinIdx;
    size_t toBlockMin =  toBlockMinIdx;

    size_t betweenBlockMin;
    if (iBlockId == (jBlockId - 1)) {
        betweenBlockMin = fromBlockMin;
    } else {
        size_t betweenBlockMinIdx = this->sparseRmq(iBlockId + 1, jBlockId - 1,
                0, this->blocksMin_, this->globals_);
        betweenBlockMin = this->blocksMinIdx_[betweenBlockMinIdx];
    }

    size_t idxs[] = {betweenBlockMin, toBlockMin};
    size_t tmpMin = this->levels_[fromBlockMin];
    size_t tmpRet = this->eulers_[fromBlockMin];
    for(int i = 0; i < 2; ++i) {
        size_t curMin = this->levels_[idxs[i]];
        if (tmpMin > curMin) {
            tmpMin = curMin;
            tmpRet = this->eulers_[idxs[i]];
        }
    }
    return tmpRet;
}

size_t RmqWrapper::getBlockId(size_t idx) const {
    return idx / this->blockSize_;
}

size_t RmqWrapper::getBlockOffset(size_t idx) const {
    return idx % this->blockSize_;
}

std::ostream& operator<<(std::ostream &strm , const RmqWrapper &rmq)
{
    std::string indent = "  ";

    strm << "RmqWrapper{" << std::endl
        << indent << "        block size: "<< rmq.blockSize_ << std::endl
        << indent << "       block count: "<< rmq.blockCount_ << std::endl
        << indent << "intervals in block: "<< rmq.intsInBlock_ << std::endl
        << indent << "            eulers: "<< rmq.eulers_ << std::endl
        << indent << "            firsts: "<< rmq.firsts_ << std::endl
        << indent << "            levels: "<< rmq.levels_ << std::endl
        << indent << "        blocks min: "<< rmq.blocksMin_ << std::endl
        << indent << "    blocks min idx: "<< rmq.blocksMinIdx_ << std::endl
        << indent << "           globals: "<< rmq.globals_ << std::endl
        << indent << "            mapper: "<< rmq.blockMapper_ << std::endl
        << indent << "            locals: "<< rmq.locals_ << std::endl;
    strm << "}" ;

    return strm;
}

    } /* rmq */
} /* tokext */
