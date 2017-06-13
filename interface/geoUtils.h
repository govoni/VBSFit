#ifndef geoUtils_h
#define geoUtils_h

#include "legendre.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include "TF1.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "THStack.h"
#include "TVector3.h"
#include "TLegend.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TMath.h"
#include "TTree.h"
#include "TROOT.h"
#include "TPad.h"
#include "TLegendEntry.h"
#include "Math/IFunction.h"
#include "TSystem.h"
#define Nm 200
#define INF 100000000.



//rotates p into the frame with W along Z axis
void invertXY (TLorentzVector & p) ;

// the histogram must be normalised s.t. \int_{-1}^{+1} dx F (x) = 1 
double coeff_legendre (TH1F* theta, unsigned int a) ;

// the histogram  must not be normalised
double coeff_legendre_new (TH1F* theta, unsigned int a) ;

// the histogram  must not be normalised
double coeff_legendre_ERR(TH1F* theta, unsigned int a) ;

double integral (TH1F* theta) ;

double L2norm (TH1F* theta) ;

double fform (double x, double f0, double fL, double fR) ;

void fillexp (TH1F* theta, double f0, double fL, double fR) ;

double deltaPhi (double phi1, double phi2) ;

//rotates p into the frame with W along Z axis
void rotIntoW (TLorentzVector & p, const TLorentzVector & W) ;

void boostIntoW (TLorentzVector & p, const TLorentzVector & W) ;

void printTLV (TString aa , const TLorentzVector & ff) ;

#endif

