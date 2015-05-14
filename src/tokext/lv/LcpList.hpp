
#ifndef LCP_LIST_HPP_M2GBYETV
#define LCP_LIST_HPP_M2GBYETV


#include <vector>
#include <deque>
#include <map>
#include <ostream>

#include "../types.hpp"

#include "Interval.hpp"
#include "Index.hpp"

// TODO:
// 1) zamienic map intptr_ na vector o stalym rozmiarze MAX_LCP <- trzeba obliczyć
// 2) trzymać vector/list o stalym rozmiarze totalFlowCount z indeksami
//      sufiksów, a Interval zamienić na iterator pierwszy, ostatni
//
//      - wtedy textptr_ ma iteratoray na vector/list i usuniecie sufixa
//      to wywalenie iteratora z listy i zmniejszenie interwalu o 1
//      a moze nawet poprawnienie jego końca lub początku
//
// (24/10/14 11:27:53 - pawel)

namespace tokext {
    namespace lv {


class LcpList
{
private:
    typedef std::deque<Interval> list_t;
    typedef list_t::iterator iterator;
    typedef list_t::const_iterator const_iterator;

public:
    LcpList(idx_t flows, idx_t suspicious, idx_t maxLcp, Index& index);

    void update(idx_t delTextIdx,
            idx_t newIdx,
            idx_t newLcp);

    idx_t idxOfSuffix(idx_t textIdx) const;

    void lcpUpdate(idx_t lcp);

    /**
     * @brief updates lcp_list is the second phase in one LCP iteration
     *
     * removes Item under textptr[delIdx], that belongs to interval
     * intptr[delLcp], then puts new Item e that has e.idx = putIdx
     * and lcp = putLcp
     *
     * @param delLcp
     * @param delIdx
     * @param putIdx
     * @param putLcp
     */
    void listUpdate(
            idx_t delIntLcp, idx_t delTextIdx, bool delIsSuspicious,
            idx_t putIdx, idx_t putLcp, bool putIsSuspicious);
private:
    idx_t lcpBound_;

    Index& index_;

    list_t intervals_;

    // Used for finding right interval to delete item from
    std::vector<iterator> intptr_;
    //std::map<idx_t, iterator> intptr_;
    // Used for finding right item to remove from interval
    std::vector<Interval::iterator> textptr_;


    idx_t removedIdx(idx_t textIdx);

    friend std::ostream& operator<<(std::ostream&, const LcpList&);
public:

    idx_t size() const {
        return this->intervals_.size();
    }

    const Index& index() const {
        return index_;
    }
};

std::ostream& operator<<(std::ostream&, const LcpList&);

    } /* lv */
} /* tokext */

#endif /* end of include guard: LCP_LIST_HPP_M2GBYETV */
