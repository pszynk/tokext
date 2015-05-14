
#ifndef TOKEN_HPP_IHANG81P
#define TOKEN_HPP_IHANG81P

#include <ostream>

#include "types.hpp"
#include "Occurrence.hpp"
#include "lv/Interval.hpp"

namespace tokext
{

class Token
{
public:
    typedef tokext::idx_t idx_t;

    Token(idx_t firstSufIdx = 0, idx_t lastSufIdx_ = 0, idx_t len = 0,
            Occurrence occurrence = {});

    Token(idx_t len, const Token &parent);

    idx_t firstSufIdx() const;
    idx_t lastSufIdx() const;
    idx_t length() const;
    Occurrence occurrence() const;

    bool operator==(const Token &other) const;
    bool operator!=(const Token &other) const;

private:
    idx_t firstSufIdx_;
    idx_t lastSufIdx_;
    idx_t length_;
    Occurrence occurrence_;

    friend std::ostream& operator<<(std::ostream &strm , const Token &token);
};


inline Token::Token(idx_t firstSufIdx, idx_t lastSufIdx, idx_t len, Occurrence occurrence):
      firstSufIdx_(firstSufIdx), lastSufIdx_(lastSufIdx), length_(len), occurrence_(occurrence)
{}

inline Token::Token(idx_t len, const Token &parent):
    Token(parent.firstSufIdx(), parent.lastSufIdx(), len, parent.occurrence())
{}


inline idx_t Token::length() const
{
    return length_;
};

inline Occurrence Token::occurrence() const
{
    return occurrence_;
};

inline idx_t Token::firstSufIdx() const
{
    return firstSufIdx_;
};

inline idx_t Token::lastSufIdx() const
{
    return lastSufIdx_;
};

std::ostream& operator<<(std::ostream &strm , const Token &token);

} /* tokext */

#endif /* end of include guard: TOKEN_HPP_IHANG81P */

