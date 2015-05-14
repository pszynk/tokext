
#ifndef EXTRACTIONPARAMS_HPP_BCAOA7V5
#define EXTRACTIONPARAMS_HPP_BCAOA7V5

#include "types.hpp"
#include "Occurrence.hpp"
#include "Token.hpp"

#include <ostream>

namespace tokext
{


class ExtractionParams
{
public:
    ExtractionParams(idx_t reqLength, Occurrence reqOccurrence);

    idx_t reqLength() const;
    Occurrence reqOccurrence() const;

    bool accept(const Token& token);

private:
    const idx_t reqLength_;
    const Occurrence reqOccurrence_;
};

inline ExtractionParams::ExtractionParams(idx_t reqLength, Occurrence reqOccurrence):
    reqLength_(reqLength), reqOccurrence_(reqOccurrence)
{}

inline idx_t ExtractionParams::reqLength() const
{
    return reqLength_;
}

inline Occurrence ExtractionParams::reqOccurrence() const
{
    return reqOccurrence_;
}

inline bool ExtractionParams::accept(const Token& token)
{
    return (token.length() >= reqLength_)
        && (token.occurrence() >= reqOccurrence_);
}

inline std::ostream& operator<<(std::ostream &strm , const ExtractionParams &params)
{
    std::string indent = "  ";

    strm << "ExtractionParams{" << std::endl
        << indent <<  "    length: "<< params.reqLength() << std::endl
        << indent <<  "occurrence: "<< params.reqOccurrence() << std::endl;

    return strm;
}

} /* tokext */

#endif /* end of include guard: EXTRACTIONPARAMS_HPP_BCAOA7V5 */
