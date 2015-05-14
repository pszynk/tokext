
#ifndef INTEGRATED_BUFFER_HPP_BKXK0PLY
#define INTEGRATED_BUFFER_HPP_BKXK0PLY

#include "types.hpp"

#include <vector>
#include <iterator>
#include <ostream>

namespace tokext
{

class InputData;

class IntegratedBuffer
{
public:
    // TODO: needs:
    // suspicious -> count and data
    // normal -> count and data
    // InputData has above ^ (06/10/14 17:59:12 - pawel)
    // Other constructor?
    IntegratedBuffer(const InputData& indata);
    //virtual ~IntegratedBuffer();

    typedef std::vector<xchar_t>::const_iterator iterator;
    iterator begin() const;
    iterator end() const;


    const xchar_t& operator[] (idx_t idx) const;

    // total size of buffer
    idx_t size() const;

    // all flows in buffer
    idx_t totalFlowCount() const;
    // suspicious flows in buffer
    idx_t suspiciousFlowCount() const;
    // normal flows in buffer
    idx_t normalFlowCount() const;

    // shift of char value (No of total flows + 1)
    idx_t shift() const;
    // greatest char val in buffer (shifted)
    idx_t alphabetSize() const;

    // idx of char thats is the beginning of flow number wno
    idx_t flowBegin(flow_idx_t wno) const;
    // idx of char thats is the ending of flow number wno
    idx_t flowEnd(flow_idx_t wno) const;

    // is flow suspicious
    bool isSuspicious(flow_idx_t idx) const;
    // which flow does this (idx) char belongs to
    flow_idx_t flowGlobalIdx(idx_t idx) const;

    void freeNormalData() {
        beginnings_.resize(suspiciousFlowCount_);
        data_.resize(suspDataSize_);
    }

private:
    // total count of integrated flows
    idx_t totalFlowCount_;
    // count of suspicious integrated flows
    idx_t suspiciousFlowCount_;
    // count of normal integrated flows
    idx_t normalFlowCount_;
    // alphabet shift
    idx_t shift_;
    // alphabet size (with data indexing)
    idx_t alphabetSize_;
    // suspicious data size
    idx_t suspDataSize_;

    // beginnings of flows in data_
    std::vector<idx_t> beginnings_;

    // integrated flow data NUMBERED FORM 1 !
    // [susp flow1]#1[susp flow2]#2 ... [susp flowM]#M ...
    // [norm flow1]#M+1 ... [norm flowN]#M+N
    std::vector<xchar_t> data_;


    friend std::ostream& operator<<(std::ostream&, const IntegratedBuffer&);

};

inline IntegratedBuffer::iterator IntegratedBuffer::begin() const
{
    return data_.cbegin();
}

inline IntegratedBuffer::iterator IntegratedBuffer::end() const
{
    return data_.cend();
}

inline const xchar_t& IntegratedBuffer::operator[] (idx_t idx) const
{
    return data_[idx];
}

inline idx_t IntegratedBuffer::size() const
{
    return data_.size();
}

inline idx_t IntegratedBuffer::totalFlowCount() const
{
    return totalFlowCount_;
}

inline idx_t IntegratedBuffer::suspiciousFlowCount() const
{
    return suspiciousFlowCount_;
}

inline idx_t IntegratedBuffer::normalFlowCount() const
{
    return normalFlowCount_;
}

inline idx_t IntegratedBuffer::shift() const
{
    return shift_;
}

inline idx_t IntegratedBuffer::alphabetSize() const
{
    return alphabetSize_;
}

inline idx_t IntegratedBuffer::flowBegin(flow_idx_t wno) const
{
    return beginnings_[wno];
}

inline idx_t IntegratedBuffer::flowEnd(flow_idx_t wno) const
{
    if (wno == totalFlowCount() - 1) {
        return size() - 1;
    }
    return beginnings_[wno + 1];
}

std::ostream& operator<<(std::ostream&, const IntegratedBuffer&);

} /* tokext */


#endif /* end of include guard: INTEGRATED_BUFFER_HPP_BKXK0PLY */
