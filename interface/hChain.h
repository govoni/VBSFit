/**
This is a container storing a chain of histograms of the same variable.
Each histogram corresponds to a given level of selections, 
the different levels of selections are identified by indices.
Histograms are by default saved in binned format, they can also be saved in an unbinned format.
*/

#ifndef hChain_h
#define hChain_h

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TNtuple.h"


struct hChain 
{
  hChain (TString baseName, TString baseTitle, 
          int nbins, double min, double max, int NUM,
          bool saveNtuple = false) ;          
  ~hChain () ;
  
  void SetColors (std::vector<int> colors) ;
  void Fill (int i, double val, double weight = 1) ;
  void Print (bool isLog = false, int rebin = 1, TString altName = "default") ;
  void PrintEach (bool isLog = false, int rebin = 1, TString altName = "default") ;
  void Scale (int index, double factor) ;
  void Write (TFile & outputFile) ;
      
  private :
  
    TString m_baseName ;
    std::vector <TH1F*> m_histos ;
    std::vector <TNtuple*> m_ntuples ;
    bool m_saveNtuple ;

    double findNMin () ;  
    double findNMax () ;
} ;

#endif
