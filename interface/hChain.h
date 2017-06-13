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
          int nbins, double min, double max, int NUM) ;          
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

    double findNMin () ;  
    double findNMax () ;
} ;

#endif
