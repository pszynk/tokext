
#include "InputBuffer.hpp"
#include "InputData.hpp"
#include "SuffixContext.hpp"
#include "utils/extraction.hpp"
#include "utils/translation.hpp"
#include <random>
#include <iostream>

#include <prettyprint/prettyprint.h>

#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/basicconfigurator.h>

using namespace tokext;
//using namespace tokext::lv;
//using namespace tokext::rmq;
using namespace std;

#define MAXN 100

///{{{1
void computeTree(int A[MAXN], int N, int T[MAXN])
  {
      int st[MAXN], i, k, top = -1;
      string indent = "   ";
  //we start with an empty stack
  //at step i we insert A[i] in the stack
      for (i = 0; i < N; i++)
      {
          cout << "{" << endl << indent << "i: " << i << endl;
          cout << indent << "top: " << top << endl;
  //compute the position of the first element that is
  //equal or smaller than A[i]
          k = top;
          cout << indent << indent << "k: " << k << " {" << endl;
          while (k >= 0 && A[st[k]] > A[i]) {
              cout << indent << indent << "k: " << k << endl;
              cout << indent << indent << "A[st[k]] > A[i] ? : " << A[st[k]] << " > " << A[i] << endl;
              k--;
          }
          cout << indent << "}" << endl;
  //we modify the tree as explained above
         if (k != -1) {
            cout << indent << "T[i] = st[k] : " << "T["<<i<<"] = " << st[k] << endl;
              T[i] = st[k];
         }
         if (k < top) {
            cout << indent << "T[st[k + 1]] = i : " <<"T["<<st[k+1]<<"] = " << i << endl;
              T[st[k + 1]] = i;
         }
  //we insert A[i] in the stack and remove
  //any bigger elements
          st[++k] = i;
          cout << indent << "st[++k] : " << "st["<<k<<"] =" << i << endl;
          top = k;
          cout << indent << "top = " << k << endl;
          cout << "}" << endl;
      }
  //the first element in the stack is the root of
  //the tree, so it has no father
      T[st[0]] = -1;
  }
///!}}}

#include "ExtractionParams.hpp"

#include "TokenUtility.hpp"

#include <random>

template<typename E>
class NumberRandomizer {
public:
    NumberRandomizer(E lower, E upper):
        distribution_(lower, upper)
    {};
    NumberRandomizer(E constant):
        noRandom_(true), constant_(constant)
    {};

    template<class URGN>
    E generate(URGN& g)
    {
        return (noRandom_ ? constant_ : distribution_(g));
    }
private:
    bool noRandom_ = false;
    E constant_ = 0;
    std::uniform_int_distribution<E> distribution_;

};

template<typename E>
class VectorRandomizer {
public:
    VectorRandomizer(
            NumberRandomizer<E> valRandomizer,
            NumberRandomizer<size_t> lenRandomizer):
        valRandomizer_(valRandomizer), lenRandomizer_(lenRandomizer)
    {};

    template<class URGN>
    std::vector<E> generate(URGN& g)
    {
        std::vector<E> vec;
        size_t len = lenRandomizer_.generate(g);
        for(size_t i = 0; i != len; ++i) {
            vec.push_back(valRandomizer_.generate(g));
        }
        return vec;
    }

private:
    NumberRandomizer<E> valRandomizer_;
    NumberRandomizer<size_t> lenRandomizer_;
};

#include <ostream>
class InputDataRandomizer {
public:

    InputDataRandomizer(VectorRandomizer<tokext::ichar_t> vectorRandomizer):
        vectorRandomizer_(vectorRandomizer)
    {};

    template<class URGN>
    tokext::InputData generate(URGN& g, size_t suspicious, size_t normal)
    {
        tokext::InputData data;
        for(size_t i = 0; i < suspicious; ++i) {
            data.putSuspicious(vectorRandomizer_.generate(g));
        }
        for(size_t i = 0; i < normal; ++i) {
            data.putNormal(vectorRandomizer_.generate(g));
        }
        return data;
    }
private:
    VectorRandomizer<tokext::ichar_t> vectorRandomizer_;
};


using namespace tokext;

int main(int argc, char **argv)
{

 // Load XML configuration file using DOMConfigurator
    log4cxx::xml::DOMConfigurator::configure("Log4cxxConfig.xml");
/*
 *    //log4cxx::BasicConfigurator::configure();
 */

    //InputData idata1 = makeInputData<vector<string>>({"acac", "aac", "caac"}, {"caac"});
    //InputData idata1 = makeInputData<vector<string>>({"acac", "aac", "caac"}, {});
    //InputData idata1 = makeInputData<vector<string>>({}, {"acac", "aac", "caac"});
    //InputData idata3 = make_InputData({"asd", "asd"}, {"asd"});

    InputData idata = makeInputData<vector<string>>({"abcdef", "xabcdefz", "bcd"}, {});
    //std::mt19937 generator;
    //    InputDataRandomizer inputGen(
    //            VectorRandomizer<ichar_t>(
    //                NumberRandomizer<ichar_t>(0, 20),
    //                NumberRandomizer<size_t>(500)));
    //TokenUtility tu(inputGen.generate(generator, 1000, 0));
    //SuffixContext cx(idata1);
    //cout << cx << endl << endl << "validate = " << cx.validate() << endl;
    //cx.validate();
    SuffixContext scx(idata);
    ExtractionParams params(3, 2);
    auto tokens = utils::extractTokens(scx, params);

    auto it = utils::findToken(scx, string("def"), tokens.begin(), tokens.end());
    cout << (*it) << endl;
    cout << utils::translateToken<string>(scx, (*it)) << endl;
    return 0;
}
