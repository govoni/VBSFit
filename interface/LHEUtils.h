#ifndef LHEUtils_h
#define LHEUtils_h

#include "TLorentzVector.h"

struct part
{
  part () :
    p (0., 0., 0., 0.),
    q (0)
      {}

  part (const TLorentzVector & p_ctor, int q_ctor) :
    p (p_ctor),
    q (q_ctor)
      {}

  TLorentzVector p ;
  int q ;
};


#endif