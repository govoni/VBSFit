#include "hFactory.h"
#include "TString.h"
#include <iostream>


hFactory::hFactory (std::string fileName, 
                    bool print) :
 m_fileName (fileName) ,
 m_print (print)
{}


//PG --------------------------------------------------------   


hFactory::~hFactory () 
{
//  if (m_H1content.size () && m_print)
//    for (std::map <TString, hChain *>::iterator mapIt = m_H1content.begin () ;
//         mapIt != m_H1content.end () ;
//         ++mapIt)
//      {
//        mapIt->second->Print (0,4) ; 
//        //PG FIXME generalizzare le opzioni di stampa    
//      }
  if (m_H1content.size () && m_fileName != "no")
    {
      TFile histosFile (m_fileName.c_str (),"recreate") ;
      histosFile.cd () ;
      for (std::map <TString, hChain *>::iterator mapIt = m_H1content.begin () ;
           mapIt != m_H1content.end () ;
           ++mapIt)
        {
          mapIt->second->Write (histosFile) ; 
        }
      histosFile.Close () ;
    }  
}


//PG --------------------------------------------------------   


void
hFactory::add_h1 (TString baseName, TString baseTitle, 
                  int nbins, double min, double max, int NUM, bool saveNtuple) 
{
  if (m_H1content.find (baseName) != m_H1content.end ())
    {
      std::cerr << "ERROR : histograms series " << baseName
                << " already existing, NOT replaced" << std::endl ;
      return ;                
    }
  hChain * dummy = new hChain (baseName, baseTitle, 
                               nbins, min, max, NUM, saveNtuple) ;
  m_H1content[baseName] = dummy ;     
  return ;                          
}


//PG --------------------------------------------------------   


void 
hFactory::Fill (const TString & name, int i, double val, double weight) 
  {
    if (m_H1content.find (name) != m_H1content.end ())
      m_H1content[name]->Fill (i, val, weight) ;
    return ;
  }


//PG --------------------------------------------------------   


hChain * 
hFactory::operator[] (const TString& name)
  {
    if (m_H1content.find (name) == m_H1content.end ())
      {
        std::cerr << "ERROR : histograms series " << name
                  << " not existing, returning the first one" << std::endl ;
        return m_H1content.begin ()->second ;                
      }
    return m_H1content[name] ;
  
  }


//PG --------------------------------------------------------   


void 
hFactory::Print (int isLog, int rebin)
{
  if (!m_H1content.size ()) return ;
  for (std::map <TString, hChain *>::iterator mapIt = m_H1content.begin () ;
       mapIt != m_H1content.end () ;
       ++mapIt)
    {
      mapIt->second->Print (isLog, rebin) ; 
    }
}
