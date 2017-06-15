/** 
example on how to read an ntuple with roofit
*/

#include "TProfile2D.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TNtuple.h"
#include <fstream>

#include "RooFormulaVar.h"
#include "RooKeysPdf.h"
#include "RooExponential.h"
#include "RooArgusBG.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooFFTConvPdf.h"

using namespace std ;

int main (int argc, char ** argv)
{
  // check number of input parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the root file --> exit " << endl ;
      return 1 ;
    }
 
  // Create variables to load from the ntuple.
  RooRealVar x ("x", "x", -1, 1) ; 
  RooRealVar w ("w", "w", 0, 100000) ; 
  RooArgSet ntupleVarSet (x, w) ;

  TFile hFile (argv[1]) ; 

  TNtuple * SM_L_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_L_cosThetaEl") ;
  TNtuple * SM_0_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_0_cosThetaEl") ;
  TNtuple * SM_N_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_NP_cosThetaEl") ;
  TNtuple * SM_R_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_R_cosThetaEl") ; 

  TNtuple * NH_L_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_L_cosThetaEl") ;
  TNtuple * Nn_0_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOn_0_cosThetaEl") ;
  TNtuple * NH_N_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_NP_cosThetaEl") ;
  TNtuple * NH_R_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_R_cosThetaEl") ; 

  RooDataSet * SM_L_DS = new RooDataSet ("SM_L_DS", "SM_L_DS", SM_L_cosThetaEl, ntupleVarSet) ;
  
  return 0 ;
}  
