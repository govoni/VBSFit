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

#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooKeysPdf.h"
#include "RooExponential.h"
#include "RooArgusBG.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooFFTConvPdf.h"

using namespace std ;
using namespace RooFit ;

int main (int argc, char ** argv)
{
  // check number of input parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the root file --> exit " << endl ;
      return 1 ;
    }
 
  // Create variables to load from the ntuple.
  RooRealVar var ("var", "var", -1, 1) ; 
  RooRealVar weight ("weight", "weight", 0, 100000) ; 
  RooArgSet ntupleVarSet (var, weight) ;

  TFile hFile (argv[1]) ; 

  TNtuple * SM_L_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_L_cosThetaEl") ;
  TNtuple * SM_0_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_0_cosThetaEl") ;
  TNtuple * SM_N_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_NP_cosThetaEl") ;
  TNtuple * SM_R_cosThetaEl = (TNtuple *) hFile.Get ("n_0_SM_R_cosThetaEl") ; 

  TNtuple * NH_L_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_L_cosThetaEl") ;
  TNtuple * NH_0_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_0_cosThetaEl") ;
  TNtuple * NH_N_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_NP_cosThetaEl") ;
  TNtuple * NH_R_cosThetaEl = (TNtuple *) hFile.Get ("n_0_NOH_R_cosThetaEl") ; 

  RooDataSet * SM_L_DS = new RooDataSet ("SM_L_DS", "SM_L_DS", SM_L_cosThetaEl, ntupleVarSet) ;
  RooDataSet * SM_0_DS = new RooDataSet ("SM_0_DS", "SM_0_DS", SM_0_cosThetaEl, ntupleVarSet) ;
  RooDataSet * SM_R_DS = new RooDataSet ("SM_R_DS", "SM_R_DS", SM_R_cosThetaEl, ntupleVarSet) ;
  RooDataSet * SM_N_DS = new RooDataSet ("SM_N_DS", "SM_N_DS", SM_N_cosThetaEl, ntupleVarSet) ;

  RooDataSet * NH_L_DS = new RooDataSet ("NH_L_DS", "NH_L_DS", NH_L_cosThetaEl, ntupleVarSet) ;
  RooDataSet * NH_0_DS = new RooDataSet ("NH_0_DS", "NH_0_DS", NH_0_cosThetaEl, ntupleVarSet) ;
  RooDataSet * NH_R_DS = new RooDataSet ("NH_R_DS", "NH_R_DS", NH_R_cosThetaEl, ntupleVarSet) ;
  RooDataSet * NH_N_DS = new RooDataSet ("NH_N_DS", "NH_N_DS", NH_N_cosThetaEl, ntupleVarSet) ;

  //FIXME reweight the datasets by the XS 

/*
  RooPlot *xplotSM = var.frame () ;
  SM_L_DS->plotOn (xplotSM, MarkerColor (kRed)) ;
  SM_0_DS->plotOn (xplotSM, MarkerColor (kBlue)) ;
  SM_N_DS->plotOn (xplotSM, MarkerColor (kGray+2)) ;
  SM_R_DS->plotOn (xplotSM, MarkerColor (kGreen+2)) ;
  xplotSM->Draw () ;
  c1.Print ("readNtuple_SM.pdf","pdf") ;

  RooPlot *xplotNH = var.frame () ;
  NH_L_DS->plotOn (xplotNH, MarkerColor (kRed)) ;
  NH_0_DS->plotOn (xplotNH, MarkerColor (kBlue)) ;
  NH_N_DS->plotOn (xplotNH, MarkerColor (kGray+2)) ;
  NH_R_DS->plotOn (xplotNH, MarkerColor (kGreen+2)) ;
  xplotNH->Draw () ;
  c1.Print ("readNtuple_NH.pdf","pdf") ;
*/

  // define functions based on histograms

  // build the pdfs associated to the datasets
  RooDataHist h_SM_L_DS ("h_SM_L_DS", "h_SM_L_DS", RooArgList (var), *SM_L_DS) ;
  RooHistPdf  f_SM_L_DS ("f_SM_L_DS", "f_SM_L_DS", RooArgSet (var), h_SM_L_DS) ;

  RooDataHist h_SM_0_DS ("h_SM_0_DS", "h_SM_0_DS", RooArgList (var), *SM_0_DS) ;
  RooHistPdf  f_SM_0_DS ("f_SM_0_DS", "f_SM_0_DS", RooArgSet (var), h_SM_0_DS) ;

  RooDataHist h_SM_R_DS ("h_SM_R_DS", "h_SM_R_DS", RooArgList (var), *SM_R_DS) ;
  RooHistPdf  f_SM_R_DS ("f_SM_R_DS", "f_SM_R_DS", RooArgSet (var), h_SM_R_DS) ;

  // the coefficients for each function, for the time being as fractions of unity
  RooRealVar coef_L ("coef_L", "coef_L", 0.5, 0., 10.) ;   
  RooRealVar coef_0 ("coef_0", "coef_0", 0.5, 0., 10.) ;   
  RooRealVar coef_R ("coef_R", "coef_R", 0.5, 0., 10.) ;   

  // the fitting function, as sum of the three
  RooAddPdf fittingFunction ("fittingFunction", "fittingFunction", 
      RooArgList (f_SM_L_DS, f_SM_0_DS, f_SM_R_DS),
      RooArgList (coef_L, coef_0)
    ) ;  

  fittingFunction.fitTo (*SM_N_DS) ;

  TCanvas c1 ;
  RooPlot *xplotSM = var.frame () ;
//  SM_L_DS->plotOn (xplotSM, MarkerStyle (4), MarkerColor (kRed)) ;
//  SM_0_DS->plotOn (xplotSM, MarkerStyle (4), MarkerColor (kBlue)) ;
//  SM_R_DS->plotOn (xplotSM, MarkerStyle (4), MarkerColor (kGreen+2)) ;
  SM_N_DS->plotOn (xplotSM, MarkerColor (kGray+2)) ;
  fittingFunction.plotOn (xplotSM, LineColor (kGreen)) ;
  fittingFunction.plotOn (xplotSM, Components (f_SM_L_DS), LineStyle (kDashed), LineColor (kRed)) ;
  fittingFunction.plotOn (xplotSM, Components (f_SM_0_DS), LineStyle (kDashed), LineColor (kBlue)) ;
  fittingFunction.plotOn (xplotSM, Components (f_SM_R_DS), LineStyle (kDashed), LineColor (kGreen+2)) ;
  xplotSM->Draw () ;
  c1.Print ("firstRooFit_SM.pdf","pdf") ;

/*
  RooPlot *xplot = var.frame () ;
  SM_L_DS->plotOn (xplot, MarkerColor (kRed)) ;
  h_SM_L_DS.plotOn (xplot, MarkerColor (kBlue)) ;
  f_SM_L_DS.plotOn (xplot, LineColor (kBlack)) ;
  xplot->Draw () ;
  c1.Print ("firstRooFit.pdf","pdf") ;
*/


  return 0 ;
}  
