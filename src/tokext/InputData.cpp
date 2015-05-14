#include "InputData.hpp"

#include <string>
#include <prettyprint/prettyprint.h>

namespace tokext
{

InputData::InputData(data_t_ suspicious, data_t_ normal):
    suspicious_(suspicious), normal_(normal)
    {
        for(auto& b : this->suspicious_) {
            this->suspDataSize_ += b.size();
        }

        for(auto& b : this->normal_) {
            this->normDataSize_ += b.size();
        }
    }

std::ostream& operator<<(std::ostream& strm, const InputData& idata)
{
    std::string indent = "  ";
    idx_t glob = 0, loc = 0;

   strm << "InputData{" << std::endl
        << indent << "suspiciousFlowCount: "<< idata.suspiciousFlowCount() << std::endl
        << indent << "    normalFlowCount: "<< idata.normalFlowCount() << std::endl
        << indent << "suspicious-----: " << std::endl;
   for(auto it = idata.beginSuspicious(); it != idata.endSuspicious(); ++it) {
       strm << "[" << loc++ << "|" << glob++ << "]" << (*it) << std::endl;;
   }
   loc = 0;
   strm << indent << "    normal-----: " << std::endl;
   for(auto it = idata.beginNormal(); it != idata.endNormal(); ++it) {
       strm << "[" << loc++ << "|" << glob++ << "]" << (*it) << std::endl;;
   }
   return strm;
}

} /* tokext */
