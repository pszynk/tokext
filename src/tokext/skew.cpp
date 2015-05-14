/**
 * This algorithm is implemented according to
 *
 * "J. Kärkkäinen and P. Sanders. Simple linear work suffix array construction.
 * In Proc. 30th idx_ternational Colloquium on Automata, Languages and Programming, volume 2719
 * of Lecture Notes in Computer Science, pages 943-955, Berlin, 2003, Springer-Verlag."
 *
 */
#include "skew.hpp"

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <iostream>

namespace tokext {
    namespace skew {


// local {{{1
namespace
{
// ? (a1, a2) <= (b1, b2)
inline bool leq(
        idx_t a1,
        idx_t a2,
        idx_t b1,
        idx_t b2)
{
  return(a1 < b1 || (a1 == b1 && a2 <= b2));
}

// ? (a1, a2, a3) <= (b1, b2, b3)
inline bool leq(
        idx_t a1,
        idx_t a2,
        idx_t a3,
        idx_t b1,
        idx_t b2,
        idx_t b3)
{
  return(a1 < b1 || (a1 == b1 && leq(a2,a3, b2,b3)));
}

static void radixPass(
        idx_t* a, // indexes in concat
        idx_t* b, // sorted indexes
        char_t* r, // concat
        idx_t n,
        char_t alphabetSize)
{
    //cout << "<<< RADIX " << endl;
    //cout << "n = " << n << endl;
    //cout << "alphabet size = " << alphabetSize << endl;
    //for (unsigned i = 0; i < n; i++) {
        //cout << "r["<<a[i]<<"] = "<<r[a[i]] <<", ";
    //}
    //cout << endl;

    // (alphabet size) buckets + 1
    char_t *c = new char_t[alphabetSize + 1];
    memset(c, 0, sizeof(char_t) * (alphabetSize + 1));

    // count buckets sizes
    for (idx_t i = 0;  i < n;  i++)
    {
        c[r[a[i]]]++;
    }

    // add previous bucket sizes
    for (idx_t i = 0, sum = 0;  i <= alphabetSize;  i++)
    {
        idx_t t = c[i];
        c[i] = sum;
        sum += t;
    }

    for (idx_t i = 0;  i < n;  i++)
    {
        b[c[r[a[i]]]++] = a[i];
    }

    delete [] c;
}


// find the suffix array SA of s[0..n-1] in {1..TX_ALPHABET_SIZE}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(
        char_t* s,
        idx_t* SA,
        idx_t n,
        char_t alphabetSize)
{
  idx_t // with max x00 ending
      n0=(n+2)/3,   // how much 0 triples
      n1=(n+1)/3,   // how much 1 triples
      n2=n/3,       // how much 2 triples
      n02=n0+n2;

  char_t* s12  = new char_t[n02 + 3];  s12[n02]= s12[n02+1]= s12[n02+2]=0;
  idx_t* SA12 = new idx_t[n02 + 3]; SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;

  char_t* s0   = new char_t[n0];
  idx_t* SA0  = new idx_t[n0];



  for (idx_t i=0, j=0;  i < n+(n0-n1);  i++) if (i%3 != 0) s12[j++] = i;

  // lsb radix sort the mod 1 and mod 2 triples
  radixPass(s12 , SA12, s+2, n02, alphabetSize);
  radixPass(SA12, s12 , s+1, n02, alphabetSize);
  radixPass(s12 , SA12, s  , n02, alphabetSize);

  // find lexicographic names of triples
  char_t name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (idx_t i = 0;  i < n02;  i++) {
    if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) {
      name++;  c0 = s[SA12[i]];  c1 = s[SA12[i]+1];  c2 = s[SA12[i]+2];
    }
    if (SA12[i] % 3 == 1) { s12[SA12[i]/3]      = name; } // left half
    else                  { s12[SA12[i]/3 + n0] = name; } // right half
  }

  // recurse if names are not yet unique
  if (name < n02) {
    suffixArray(s12, SA12, n02, name);
    // store unique names in s12 using the suffix array
    for (idx_t i = 0;  i < n02;  i++) s12[SA12[i]] = i + 1;
  } else // generate the suffix array of s12 directly
    for (idx_t i = 0;  i < n02;  i++) SA12[s12[i] - 1] = i;

  // stably sort the mod 0 suffixes from SA12 by their first character
  for (idx_t i=0, j=0;  i < n02;  i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
  radixPass(s0, SA0, s, n0, alphabetSize);

  // merge sorted SA0 suffixes and sorted SA12 suffixes
  for (idx_t p=0,  t=n0-n1,  k=0;  k < n;  k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
    idx_t i = GetI(); // pos of current offset 12 suffix
    idx_t j = SA0[p]; // pos of current offset 0  suffix
    if (SA12[t] < n0 ?
        leq(s[i],       s12[SA12[t] + n0], s[j],       s12[j/3]) :
        leq(s[i],s[i+1],s12[SA12[t]-n0+1], s[j],s[j+1],s12[j/3+n0]))
    { // suffix from SA12 is smaller
      SA[k] = i;  t++;
      if (t == n02) { // done --- only SA0 suffixes left
        for (k++;  p < n0;  p++, k++) SA[k] = SA0[p];
      }
    } else {
      SA[k] = j;  p++;
      if (p == n0)  { // done --- only SA12 suffixes left
        for (k++;  t < n02;  t++, k++) SA[k] = GetI();
      }
    }
  }
  delete [] s12; delete [] SA12; delete [] SA0; delete [] s0;
}

} /* __local__ */
// 1}}}

// zwraca vector<idx_t>
// dostaje concat i tworzy wlasny, dodaje 000
// rozmmiar alfabetu
std::vector<idx_t> skewSuffixArray(
        vec_t buffer,
        idx_t alphabetSize)
{
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(0);

    std::vector<idx_t> sa;
    sa.resize(buffer.size());

    suffixArray(
             buffer.data(),
             sa.data(),
             buffer.size() - 3,
             alphabetSize);

    sa.pop_back();
    sa.pop_back();
    sa.pop_back();
    return sa;
}

    } /* skew */
} /* tokext */
