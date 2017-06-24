#ifndef utils_h
#define utils_h

#include <functional>

struct ptRevSort: public std::binary_function<const TLorentzVector &, const TLorentzVector &, bool>
{
  bool operator() (const part & x, const part & y)
    {
      return x.p.Pt () > y.p.Pt () ;
    }
} ;

#endif