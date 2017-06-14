/**
read a LHE file extracting the lorentz vectors of the initial and final state particles,
as well as their charge, using a config file that contains the relevant information
for the origin file, the cross-section, the number of events to be read.
Operate on e mu final state, test of the rotations.
Fill the cos(theta) histogram for the rotated lepton.
*/

#include "LHEF.h"
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
//#include "hFactory.h"
//#include "h2Factory.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "LHEUtils.h"
#include "CfgParser.h"
#include "legendre.h"
#include "geoUtils.h"
#include "hFactory.h"
#include "hFunctions.h"

using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void fillVector (vector<part> & objects, LHEF::Reader & reader, int iPart)
  {
    objects.push_back (part 
      (
        TLorentzVector
          (
            reader.hepeup.PUP.at (iPart).at (0), //PG px
            reader.hepeup.PUP.at (iPart).at (1), //PG py
            reader.hepeup.PUP.at (iPart).at (2), //PG pz
            reader.hepeup.PUP.at (iPart).at (3) //PG E
          ),
        reader.hepeup.IDUP.at (iPart)
      )) ;
    return ;
  }


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void readLHEEvent (LHEF::Reader & reader,
                   vector<part> & higgs,     
                   vector<part> & finalJets,   
                   vector<part> & initialQuarks,    
                   vector<part> & finalQuarks,     
                   vector<part> & finalGluon,      
                   vector<part> & finalLeptons,    
                   vector<part> & finalNeutrinos  
                  )
  {
    //PG loop over particles in the event
    //PG and fill the variables of leptons and quarks
    for (unsigned int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart)
      {
//        std::cout << "\t part type [" << iPart << "] " << reader.hepeup.IDUP.at (iPart)
//                  << "\t status " << reader.hepeup.ISTUP.at (iPart)
//                  << "\n" ;
        //PG incoming particle          
        if (reader.hepeup.ISTUP.at (iPart) == -1)
          {
           fillVector (initialQuarks, reader, iPart) ;
          } //PG incoming particle          

        //PG outgoing particles          
        if (reader.hepeup.ISTUP.at (iPart) == 1)
          {
            // jets  
            if (abs (reader.hepeup.IDUP.at (iPart)) < 7) // quarks
              {
                fillVector (finalJets, reader, iPart) ;
                fillVector (finalQuarks, reader, iPart) ;
              } // quarks
            else if (abs (reader.hepeup.IDUP.at (iPart)) == 21 ) // gluons
              {
                fillVector (finalJets, reader, iPart) ;
                fillVector (finalGluon, reader, iPart) ;
              } // gluons
            else if (abs (reader.hepeup.IDUP.at (iPart)) == 25) // higgs
              {
                fillVector (higgs, reader, iPart) ;
              } //PG higgs
            else if (abs (reader.hepeup.IDUP.at (iPart)) == 11 ||
                     abs (reader.hepeup.IDUP.at (iPart)) == 13) // electron or muon
              {
                fillVector (finalLeptons, reader, iPart) ;
              } // electron or muon
            else if (abs (reader.hepeup.IDUP.at (iPart)) == 12 ||
                     abs (reader.hepeup.IDUP.at (iPart)) == 14) // electron or muon neutrinos
              {
                fillVector (finalNeutrinos, reader, iPart) ;
              } // electron or muon neutrinos
            else if (abs (reader.hepeup.IDUP.at (iPart)) == 9) // gluon?
              {
                cout << "found gluon with pddgID == 9\n" ;
                fillVector (finalJets, reader, iPart) ;
              } //PG gluon?
          } //PG outgoing particles
      } //PG loop over particles in the event
    return ;  
  }


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int main (int argc, char **argv) 
{
  // check number of inpt parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  vector<string> samplesList = gConfigParser->readStringListOpt ("general", "samplesList") ;
  bool saveNtuple = gConfigParser->readBoolOpt ("general", "saveNtuple") ;

  gROOT->SetStyle ("Plain") ;

  string outputFileName = gConfigParser->readStringOpt ("general", "outputFile") ; 
  hFactory H1fact (outputFileName, true) ;
  int numOfHistos = 1 ;
  
  for (unsigned int i = 0 ; i < samplesList.size () ; ++i)
    {
      string histoName = samplesList.at (i) + "_cosThetaEl" ;
      H1fact.add_h1 (histoName, histoName, 200, -1, 1, numOfHistos, saveNtuple) ;
      histoName = samplesList.at (i) + "_cosThetaMu" ;
      H1fact.add_h1 (histoName, histoName, 200, -1, 1, numOfHistos, saveNtuple) ;
    }

  //PG loop over samples
  for (unsigned int i = 0 ; i < samplesList.size () ; ++i)
    {
      TString sampleName  = gConfigParser->readStringOpt (samplesList.at (i), "LHEfile") ;
      int maxEvents       = gConfigParser->readIntOpt (samplesList.at (i), "maxEvents") ;    
      int maxSelEvents    = gConfigParser->readIntOpt (samplesList.at (i), "maxSelEvents") ;    
      double crossSection = gConfigParser->readFloatOpt (samplesList.at (i), "crossSection") ;    

      cout << "READING:\n" ;
      cout << "  file:\t" << sampleName.Data () << endl ;
      cout << "  maxEvents:\t " << maxEvents << endl ;
      cout << "  maxSelEvents:\t" << maxSelEvents << endl ;
      cout << "  crossSection:\t" << crossSection << endl ;

      std::ifstream ifs (sampleName.Data ()) ;
      LHEF::Reader reader (ifs) ;

      int ieve = 0 ;
      // loop over events
      while ( reader.readEvent () ) 
        {
          if (ieve % 100000 == 0) std::cout << "  ... event " << ieve << "\n" ;
          if (maxEvents > 0 && ieve >= maxEvents) break ;
          ++ieve;

          vector<part> higgs ;      
          vector<part> finalJets ;      
          vector<part> initialQuarks ;      
          vector<part> finalQuarks ;      
          vector<part> finalGluon ;      
          vector<part> finalLeptons ;      
          vector<part> finalNeutrinos ;

          readLHEEvent (reader,
                        higgs,     
                        finalJets,   
                        initialQuarks,    
                        finalQuarks,     
                        finalGluon,      
                        finalLeptons,    
                        finalNeutrinos  
                       ) ;

          if (finalLeptons.size () != 2 ||
          	  finalNeutrinos.size () !=2 || 
          	  finalQuarks.size () + finalGluon.size () != 2)
            {
               cout << "WARNING, the number final state particles is strange: " ;
               cout << finalLeptons.size () << ", "
                    << finalNeutrinos.size () << ", "
                    << finalQuarks.size () << ", "
                    << finalGluon.size () ;
               cout << ". Skipping the event\n" ;
               continue ;
            }

          //PG identify the final state leptons in the lab frame (_lab)
  
          TLorentzVector * TLV_mu_lab = &(finalLeptons.at (0).p) ;
          TLorentzVector * TLV_el_lab = &(finalLeptons.at (1).p) ;
          if (abs (finalLeptons.at (0).q) != 13) swap (TLV_el_lab, TLV_mu_lab) ;

          TLorentzVector * TLV_vm_lab = &(finalNeutrinos.at (0).p) ;
          TLorentzVector * TLV_ve_lab = &(finalNeutrinos.at (1).p) ;
          if (abs (finalNeutrinos.at (0).q) != 14) swap (TLV_ve_lab, TLV_vm_lab) ;

          //PG build the intermediate W candidates in the lab frame

          TLorentzVector TLV_We_lab = *TLV_el_lab + *TLV_ve_lab ;      
          TLorentzVector TLV_Wm_lab = *TLV_mu_lab + *TLV_vm_lab ;      

          //PG apply any selections here!
          //PG ---- ---- ---- ---- ---- ---- ---- ---- ----

          //PG rotate leptons, build rotated W's, boost leptons, 
          //PG to obtain the leptons in the W frame (_rot)

          TLorentzVector TLV_el_rot = *TLV_el_lab ; 
          TLorentzVector TLV_ve_rot = *TLV_ve_lab ;      
          TLorentzVector TLV_mu_rot = *TLV_mu_lab ; 
          TLorentzVector TLV_vm_rot = *TLV_vm_lab ;      

          rotIntoW (TLV_el_rot, TLV_We_lab);
          rotIntoW (TLV_ve_rot, TLV_We_lab);
          rotIntoW (TLV_mu_rot, TLV_Wm_lab);
          rotIntoW (TLV_vm_rot, TLV_Wm_lab);                       
          
          TLorentzVector TLV_We_rot = TLV_el_rot + TLV_ve_rot ;      
          TLorentzVector TLV_Wm_rot = TLV_mu_rot + TLV_vm_rot ;      

          boostIntoW (TLV_el_rot, TLV_We_rot);
          boostIntoW (TLV_ve_rot, TLV_We_rot);
          boostIntoW (TLV_mu_rot, TLV_Wm_rot);
          boostIntoW (TLV_vm_rot, TLV_Wm_rot);                       

          H1fact.Fill (samplesList.at (i) + "_cosThetaEl", 0, cos (TLV_el_rot.Theta ())) ;
          H1fact.Fill (samplesList.at (i) + "_cosThetaMu", 0, cos (TLV_mu_rot.Theta ())) ;

        } //PG loop over events
      
      //PG normalize all histograms to the cross-section:
      //PG - the integral of the histogram returns the cross-section of the sample
      //PG - in case of selections, it returns the cross-section of the selected phase space
      //PG - therefore, bins are integral, i.e. they are not divided by the width
      double scaleFactor = crossSection / ieve ;
      H1fact.applyToSubset (scaleH1 (-1, scaleFactor), samplesList.at (i)) ;
      cout << "scaling " << samplesList.at (i) << " by " << scaleFactor << " = " << crossSection << " / " << ieve << endl ;
    } //PG loop over samples 

  delete gConfigParser ;
  return 0 ;
}

