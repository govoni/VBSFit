#ifndef hFactory_h
#define hFactory_h

#include <map>
#include <string>
#include <functional>
#include "hChain.h"
//#include "h2Chain.h"
#include "TString.h"

class hFactory
{
  public :
    hFactory (std::string fileName = "no", bool print = true) ;
    ~hFactory () ;

    void add_h1 (TString baseName, TString baseTitle, 
                 int nbins, double min, double max, int NUM) ;

    template <class UnaryFunction>
    void applyToAll (UnaryFunction function) 
      {
        for (std::map <TString, hChain *>::iterator mapIt = m_H1content.begin () ;
             mapIt != m_H1content.end () ;
             ++mapIt)
          {
            function (mapIt->second) ;
          }
      }

    void Fill (const TString & name, int i, double val, double weight = 1.) ;
    hChain * operator[] (const TString& name) ;
    void Print (int isLog = 0, int rebin = 1) ;

  private :
  
    std::string m_fileName ;
    bool m_print ;
    std::map <TString, hChain *> m_H1content ;

} ;

#endif
