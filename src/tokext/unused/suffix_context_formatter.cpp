
#include "suffix_context_formatter.hpp"

namespace tokext
{
/*
std::string GeneralizedSuffixArray::char2alnum(xchar_t encChar) const
{
    if (encChar < shift_) {
        return "#" + std::to_string(encChar);
    }

    encChar -= shift_;
    if (isalnum(encChar)) {
        return std::string(1, encChar);
    }
    return "!";
}

std::string GeneralizedSuffixArray::char2code(xchar_t encChar) const
{
    if (encChar < shift_) {
        return "#" + std::to_string(encChar);
    }

    encChar -= shift_;
    return "\\" + std::to_string(encChar);
}

std::string GeneralizedSuffixArray::substr_formatter(idx_t idx, idx_t len, char2_fp_t trans) const
{
    std::string ret = "";
    for (idx_t i = 0; i < len; ++i)
    {
        ret.append((this->*trans)(concat_[idx+i]));
    }
    return ret;
}

std::string GeneralizedSuffixArray::table_formatter(substr_info_fp_t trans) const
{
    using namespace std;

    const char fil = ' ';
    const char vsep =  '-';
    const string hsep =  " | ";

    const string labelI = "i";
    const string labelSA = "SA[i]";
    const string labelInvSA = "SA^-1[i]";
    const string labelLCP = "LCP[i]";
    const string labelText = "text[i]";
    const string labelSubstr = "T^#_SA[i]";

    const size_t swidth = max(
            {
            to_string(size_).length(),
            labelI.length(),
            labelSA.length(),
            labelInvSA.length()
            }
    );
    const size_t twidth = max(
            {
            to_string(numWords_).length(),
            labelText.length()
            }
    );

    const size_t lwidth = max(
            {
            ((lcp_array_.size() == 0) ?
             0 :
             std::to_string(*max_element(lcp_array_.begin(), lcp_array_.end())).length()),
            labelLCP.length()
            }
    );

    stringstream ss;

    // i [idx in SA] | SA[i] | SA^-1[i] | LCP[i] | text[i] | T^#_SA[i]
    ss << hsep << right << setw(swidth) << setfill(fil) << "i"
        << hsep << right << setw(swidth) << setfill(fil) << "SA[i]"
        << hsep << right << setw(swidth) << setfill(fil) << "SA^-1[i]"
        << hsep << right << setw(lwidth) << setfill(fil) << "LCP[i]"
        << hsep << right << setw(twidth) << setfill(fil) << "text[i]"
        << hsep << "T^#_SA[i]" << endl;
    size_t totalwidth = ss.tellp();

    ss << string(totalwidth, vsep) << endl;
    for (idx_t i = 0; i < size_; ++i)
    {
        ss << hsep << right << setw(swidth) << setfill(fil) << i
            << hsep << right << setw(swidth) << setfill(fil) << suffix_array_[i]
            << hsep << right << setw(swidth) << setfill(fil) << inv_suffix_array_[i]
            << hsep << right << setw(lwidth) << setfill(fil) << lcp_array_[i]
            << hsep << right << setw(twidth) << setfill(fil) << text_array_[i];

        idx_t substrLen = ((text_array_[i] == numWords_ - 1) ?
            size_ :
            wordBegin_[text_array_[i]+1]) - suffix_array_[i];


        ss << hsep << ((this->*trans)(suffix_array_[i], substrLen));
        ss << endl;
    }

    return ss.str();
}

std::string GeneralizedSuffixArray::code_substr(idx_t idx, idx_t len) const
{
    return (substr_formatter(idx, len, &GeneralizedSuffixArray::char2code));
}

std::string GeneralizedSuffixArray::code_table() const
{
    return (table_formatter(&GeneralizedSuffixArray::code_substr));
}

std::string GeneralizedSuffixArray::alnum_substr(idx_t idx, idx_t len) const
{
    return (substr_formatter(idx, len, &GeneralizedSuffixArray::char2alnum));
}
std::string GeneralizedSuffixArray::alnum_table() const
{
    return (table_formatter(&GeneralizedSuffixArray::alnum_substr));
}
*/
} /* tokext */
