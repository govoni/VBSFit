
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include <cmath>

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "histoUtils.h"

using namespace std ;

int main (int argc, char ** argv)
{
 // check number of input parameters
 if (argc < 2)
  {
   cerr << "Forgot to put the root file --> exit " << endl ;
   return 1 ;
  }

 //PG read histograms from root file
 
 TFile hFile (argv[1]) ; 

 TH1F * SM_L_cosThetaEl = (TH1F *) hFile.Get ("h_0_SM_L_cosThetaEl") ;
 TH1F * SM_0_cosThetaEl = (TH1F *) hFile.Get ("h_0_SM_0_cosThetaEl") ;
 TH1F * SM_N_cosThetaEl = (TH1F *) hFile.Get ("h_0_SM_NP_cosThetaEl") ;
 TH1F * SM_R_cosThetaEl = (TH1F *) hFile.Get ("h_0_SM_R_cosThetaEl") ;

 TH1F * NH_L_cosThetaEl = (TH1F *) hFile.Get ("h_0_NOH_L_cosThetaEl") ;
 TH1F * NH_0_cosThetaEl = (TH1F *) hFile.Get ("h_0_NOH_0_cosThetaEl") ;
 TH1F * NH_N_cosThetaEl = (TH1F *) hFile.Get ("h_0_NOH_NP_cosThetaEl") ;
 TH1F * NH_R_cosThetaEl = (TH1F *) hFile.Get ("h_0_NOH_R_cosThetaEl") ;

 TLegend theLegend (0.3, 0.6, 0.6, 0.95) ;

 theLegend.AddEntry (NH_N_cosThetaEl , "noH, N", "P") ;
 theLegend.AddEntry (NH_L_cosThetaEl , "noH, L" , "P") ;
 theLegend.AddEntry (NH_R_cosThetaEl , "noH, R" , "P") ;
 theLegend.AddEntry (NH_0_cosThetaEl , "noH, 0" , "P") ;

 theLegend.AddEntry (SM_N_cosThetaEl , "SM, N" , "L") ;
 theLegend.AddEntry (SM_L_cosThetaEl , "SM, L" , "L") ;
 theLegend.AddEntry (SM_R_cosThetaEl , "SM, R" , "L") ;
 theLegend.AddEntry (SM_0_cosThetaEl , "SM, 0" , "L") ;

 NH_0_cosThetaEl->SetLineColor (kBlue) ;
 NH_L_cosThetaEl->SetLineColor (kGreen+2) ;
 NH_R_cosThetaEl->SetLineColor (kRed) ;
 NH_N_cosThetaEl->SetLineColor (kGray+2) ;
 
 SM_0_cosThetaEl->SetLineColor (kBlue) ;
 SM_L_cosThetaEl->SetLineColor (kGreen+2) ;
 SM_R_cosThetaEl->SetLineColor (kRed) ;
 SM_N_cosThetaEl->SetLineColor (kGray+2) ;
 
 SM_0_cosThetaEl->SetLineWidth (2) ;
 SM_L_cosThetaEl->SetLineWidth (2) ;
 SM_R_cosThetaEl->SetLineWidth (2) ;
 SM_N_cosThetaEl->SetLineWidth (2) ;

 //PG calculate the interference

 TH1F * SM_sum_cosThetaEl = (TH1F *) SM_0_cosThetaEl->Clone ("SM_sum_cosThetaEl") ;
 SM_sum_cosThetaEl->Add (SM_L_cosThetaEl) ; 
 SM_sum_cosThetaEl->Add (SM_R_cosThetaEl) ; 
 TH1F * SM_I_cosThetaEl = (TH1F *) SM_N_cosThetaEl->Clone ("SM_I_cosThetaEl") ;
 SM_I_cosThetaEl->Add (SM_sum_cosThetaEl, -1) ;
 SM_I_cosThetaEl->Rebin (10) ;

 TH1F * NH_sum_cosThetaEl = (TH1F *) NH_0_cosThetaEl->Clone ("NH_sum_cosThetaEl") ;
 NH_sum_cosThetaEl->Add (NH_L_cosThetaEl) ; 
 NH_sum_cosThetaEl->Add (NH_R_cosThetaEl) ; 
 TH1F * NH_I_cosThetaEl = (TH1F *) NH_N_cosThetaEl->Clone ("NH_I_cosThetaEl") ;
 NH_I_cosThetaEl->Add (NH_sum_cosThetaEl, -1) ;
 NH_I_cosThetaEl->Rebin (10) ;

 SM_I_cosThetaEl->SetLineColor (kPink) ;
 SM_I_cosThetaEl->SetLineWidth (2) ;
 NH_I_cosThetaEl->SetLineColor (kPink) ;

  //PG prepare normalized histograms

 TH1F * N_SM_L_cosThetaEl = (TH1F *) SM_L_cosThetaEl->Clone ("N_SM_L_cosThetaEl") ;
 TH1F * N_SM_0_cosThetaEl = (TH1F *) SM_0_cosThetaEl->Clone ("N_SM_0_cosThetaEl") ;
 TH1F * N_SM_N_cosThetaEl = (TH1F *) SM_N_cosThetaEl->Clone ("N_SM_N_cosThetaEl") ;
 TH1F * N_SM_R_cosThetaEl = (TH1F *) SM_R_cosThetaEl->Clone ("N_SM_R_cosThetaEl") ;
 TH1F * N_SM_I_cosThetaEl = (TH1F *) SM_I_cosThetaEl->Clone ("N_SM_I_cosThetaEl") ;
  
 N_SM_L_cosThetaEl->Scale (1. / N_SM_L_cosThetaEl->Integral ()) ;
 N_SM_0_cosThetaEl->Scale (1. / N_SM_0_cosThetaEl->Integral ()) ;
 N_SM_N_cosThetaEl->Scale (1. / N_SM_N_cosThetaEl->Integral ()) ; 
 N_SM_R_cosThetaEl->Scale (1. / N_SM_R_cosThetaEl->Integral ()) ; 
 N_SM_I_cosThetaEl->Scale (1. / N_SM_I_cosThetaEl->Integral ()) ; 

 TH1F * N_NH_L_cosThetaEl = (TH1F *) NH_L_cosThetaEl->Clone ("N_NH_L_cosThetaEl") ;
 TH1F * N_NH_0_cosThetaEl = (TH1F *) NH_0_cosThetaEl->Clone ("N_NH_0_cosThetaEl") ;
 TH1F * N_NH_N_cosThetaEl = (TH1F *) NH_N_cosThetaEl->Clone ("N_NH_N_cosThetaEl") ;
 TH1F * N_NH_R_cosThetaEl = (TH1F *) NH_R_cosThetaEl->Clone ("N_NH_R_cosThetaEl") ;
 TH1F * N_NH_I_cosThetaEl = (TH1F *) NH_I_cosThetaEl->Clone ("N_NH_I_cosThetaEl") ;
  
 N_NH_L_cosThetaEl->Scale (1. / N_NH_L_cosThetaEl->Integral ()) ;
 N_NH_0_cosThetaEl->Scale (1. / N_NH_0_cosThetaEl->Integral ()) ;
 N_NH_N_cosThetaEl->Scale (1. / N_NH_N_cosThetaEl->Integral ()) ; 
 N_NH_R_cosThetaEl->Scale (1. / N_NH_R_cosThetaEl->Integral ()) ; 
 N_NH_I_cosThetaEl->Scale (1. / N_NH_I_cosThetaEl->Integral ()) ; 

//PG prepare some plots

 TCanvas c0 ;
 c0.cd () ;
 
 SM_L_cosThetaEl->DrawClone ("histo") ;
 SM_0_cosThetaEl->DrawClone ("histo same") ;
 SM_N_cosThetaEl->DrawClone ("histo same") ;
 SM_R_cosThetaEl->DrawClone ("histo same") ;
 
 NH_L_cosThetaEl->DrawClone ("same") ;
 NH_0_cosThetaEl->DrawClone ("same") ;
 NH_N_cosThetaEl->DrawClone ("same") ;
 NH_R_cosThetaEl->DrawClone ("same") ;
 
 theLegend.Draw () ;
 c0.Print ("comp_summary.pdf","pdf") ;
 
 TCanvas c1 ;
 c1.Divide (3,2) ;
 c1.cd (1) ;
 SM_N_cosThetaEl->DrawClone ("histo") ;
 NH_N_cosThetaEl->DrawClone ("same") ;
 c1.cd (2) ;
 NH_0_cosThetaEl->DrawClone ("") ;
 SM_0_cosThetaEl->DrawClone ("histo same") ;
 c1.cd (3) ;
 SM_L_cosThetaEl->DrawClone ("histo") ;
 NH_L_cosThetaEl->DrawClone ("same") ;
 c1.cd (4) ;
 SM_R_cosThetaEl->DrawClone ("histo") ;
 NH_R_cosThetaEl->DrawClone ("same") ;
 c1.cd (5) ;
 SM_I_cosThetaEl->DrawClone ("histo") ;
 NH_I_cosThetaEl->DrawClone ("same") ;
 
 c1.Print ("comp_separate.pdf","pdf") ;
 
 TCanvas c2 ;
 c2.Divide (3,2) ;
 c2.cd (1) ;
 N_SM_N_cosThetaEl->DrawClone ("histo") ;
 N_NH_N_cosThetaEl->DrawClone ("same") ;
 c2.cd (2) ;
 N_SM_0_cosThetaEl->DrawClone ("histo") ;
 N_NH_0_cosThetaEl->DrawClone ("same") ;
 c2.cd (3) ;
 N_SM_L_cosThetaEl->DrawClone ("histo") ;
 N_NH_L_cosThetaEl->DrawClone ("same") ;
 c2.cd (4) ;
 N_SM_R_cosThetaEl->DrawClone ("histo") ;
 N_NH_R_cosThetaEl->DrawClone ("same") ;
 c2.cd (5) ;
 N_SM_I_cosThetaEl->DrawClone ("histo") ;
 N_NH_I_cosThetaEl->DrawClone ("same") ;

 c2.Print ("norm_comp_separate.pdf","pdf") ;
 
 TCanvas c3 ;
 c3.cd () ;
 
 N_SM_L_cosThetaEl->DrawClone ("histo") ;
 N_SM_0_cosThetaEl->DrawClone ("histo same") ;
 N_SM_N_cosThetaEl->DrawClone ("histo same") ;
 N_SM_R_cosThetaEl->DrawClone ("histo same") ;
 N_SM_I_cosThetaEl->DrawClone ("histo same") ;
 
 N_NH_L_cosThetaEl->DrawClone ("same") ;
 N_NH_0_cosThetaEl->DrawClone ("same") ;
 N_NH_N_cosThetaEl->DrawClone ("same") ;
 N_NH_R_cosThetaEl->DrawClone ("same") ;
 N_NH_I_cosThetaEl->DrawClone ("same") ;
 
 theLegend.Draw () ;
 c3.Print ("norm_comp_summary.pdf","pdf") ;


 //PG draw some comparisons

 TGraphAsymmErrors * compare_L = makeDataOverMCRatioPlot (N_SM_L_cosThetaEl, N_NH_L_cosThetaEl) ;
 TGraphAsymmErrors * compare_0 = makeDataOverMCRatioPlot (N_SM_0_cosThetaEl, N_NH_0_cosThetaEl) ;
 TGraphAsymmErrors * compare_N = makeDataOverMCRatioPlot (N_SM_N_cosThetaEl, N_NH_N_cosThetaEl) ;
 TGraphAsymmErrors * compare_R = makeDataOverMCRatioPlot (N_SM_R_cosThetaEl, N_NH_R_cosThetaEl) ;
 TGraphAsymmErrors * compare_I = makeDataOverMCRatioPlot (N_SM_I_cosThetaEl, N_NH_I_cosThetaEl) ;


 return 0 ;
}

