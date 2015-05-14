
#ifndef INDEX_HPP_XQT6PDC8
#define INDEX_HPP_XQT6PDC8


#include <map>
#include <ostream>

#include "Interval.hpp"
#include "../types.hpp"
#include "../GeneralizedSuffixArray.hpp"
#include "../Token.hpp"
#include "../Occurrence.hpp"

#include <vector>

namespace tokext
{
class ExtractionParams;

    namespace lv
    {

class Index
{
public:
    // token length -> Token
    typedef std::map<idx_t, Token> inner_map_t;
    //sa idx -> l2t_map__
    typedef std::map<idx_t, inner_map_t> outer_map_t;

    typedef outer_map_t::const_iterator iterator;

public:
    Index(ExtractionParams& params);
    bool add(Token token);

    iterator begin() const;
    iterator end() const;



    const outer_map_t& index();

private:
    outer_map_t index_;
    ExtractionParams& params_;

    bool shouldReplace_(const Token& old, const Token& arrived) const;
    void add_(Token &&token);

    bool check_(const Token& token) const;

    friend std::ostream& operator<<(std::ostream&, const Index&);
};

std::ostream& operator<<(std::ostream&, const Index&);

inline const Index::outer_map_t& Index::index()
{
    return index_;
}

inline Index::iterator Index::begin() const
{
    return index_.cbegin();
}

inline Index::iterator Index::end() const
{
    return index_.cend();
}

    } /* lv */
} /* tokext */


#endif /* end of include guard: INDEX_HPP_XQT6PDC8 */
