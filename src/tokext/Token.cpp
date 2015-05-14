
#include "Token.hpp"

#include <prettyprint/prettyprint.h>

namespace tokext
{

bool Token::operator==(const Token &other) const
{
    return (this->firstSufIdx() == other.firstSufIdx()) &&
        (this->lastSufIdx() == other.lastSufIdx()) &&
        (this->length() == other.length());
}

bool Token::operator!=(const Token &other) const
{
    return !(*this == other);
}


std::ostream& operator<<(std::ostream &strm , const Token &token)
{
    auto indent = "   ";
    return strm << "Token{" << std::endl
        << indent << "firstSufIdx: " << token.firstSufIdx_ << std::endl
        << indent << " lastSufIdx: " << token.lastSufIdx_ << std::endl
        << indent << "     length: " << token.length_ << std::endl
        << indent << "opccurrence: " << token.occurrence() << std::endl
        << "}";
}

} /* tokext */
