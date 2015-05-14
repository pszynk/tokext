
#ifndef INPUT_DATA_HPP_ZH32XELZ
#define INPUT_DATA_HPP_ZH32XELZ

#include "types.hpp"

#include <vector>
#include <iostream>
#include <initializer_list>
#include <utility>

namespace tokext
{

class InputData
{
private:
    typedef std::vector<ichar_t> buffer_t_;
    typedef std::vector<buffer_t_> data_t_;

public:
    InputData() {};
    InputData(data_t_ suspicious, data_t_ normal);

    //virtual ~InputData ();
    //
    typedef buffer_t_ buffer;

    typedef data_t_::const_iterator iterator;

    iterator beginSuspicious() const
    { return suspicious_.begin(); };

    iterator endSuspicious() const
    { return suspicious_.end(); };

    iterator beginNormal() const
    { return normal_.begin(); };

    iterator endNormal() const
    { return normal_.end(); };

    idx_t totalFlowCount() const
    { return suspiciousFlowCount() + normalFlowCount(); };

    idx_t suspiciousFlowCount() const
    { return suspicious_.size(); };

    idx_t normalFlowCount() const
    { return normal_.size(); };

    bool empty() const {
        return totalFlowCount() == 0;
    }

    std::size_t suspiciousDataSize() {
        return suspDataSize_;
    }

    std::size_t normalDataSize() {
        return normDataSize_;
    }

    void putSuspicious(buffer_t_ buffer) {
        suspDataSize_ += buffer.size();
        suspicious_.push_back(buffer);
    };


    void putNormal(buffer_t_ buffer) {
        normDataSize_ += buffer.size();
        normal_.push_back(buffer);
    };

    template<class Iter>
    void insertSuspiciousData(Iter begin_, Iter end_)
    {
        for(auto it = begin_; it != end_; ++it) {
            suspDataSize_ += it->size();
            this->suspicious_.push_back((*it));
        }
    }

    template<class Iter>
    void insertNormalData(Iter begin_, Iter end_)
    {
        for(auto it = begin_; it != end_; ++it) {
            normDataSize_ += it->size();
            this->normal_.push_back((*it));
        }
    }

private:

    std::size_t suspDataSize_ = 0;
    std::size_t normDataSize_ = 0;
    data_t_ suspicious_;
    data_t_ normal_;

    friend std::ostream& operator<<(std::ostream&, const InputData&);
};


namespace
{

template<class ConT>
InputData makeInputDataImpl(ConT&& susp, ConT&& norm)
{
    using std::begin;
    using std::end;
    InputData id;
    for(auto& s : susp) {
        id.putSuspicious({begin(s), end(s)});
    }
    for(auto& n : norm) {
        id.putNormal({begin(n), end(n)});
    }
    return id;
}

}

template<class ConT>
InputData makeInputData(ConT&& susp, ConT&& norm)
{
    return makeInputDataImpl(std::forward<ConT>(susp), std::forward<ConT>(norm));
}

//template<typename T>
//InputData make_InputData(
//        std::initializer_list<T>&& susp,
//        std::initializer_list<T>&& norm)
//{
//    return make_InputData_impl<std::initializer_list<T>&>(susp, norm);
//}
} /* tokext */

#endif /* end of include guard: INPUT_DATA_HPP_ZH32XELZ */
