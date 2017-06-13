/**
read a LHE file extracting the lorentz vectors of the initial and final state particles,
as well as their charge, using a config file that contains the relevant information
for the origin file, the cross-section, the number of events to be read
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
//#include "hFunctions.h"

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "LHEUtils.h"
#include "CfgParser.h"

using namespace std ;


double 
deltaPhi (double phi1, double phi2)
{

  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}


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

  vector<string> samplesList = gConfigParser->readStringListOpt ("general::samplesList") ;

   //PG loop over samples
  for (unsigned int i = 0 ; i < samplesList.size () ; ++i)
      {

      TString sampleName = gConfigParser->readStringOpt (samplesList.at (i), "LHEfile") ;
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
          if (ieve % 10000 == 0) std::cout << "  ... event " << ieve << "\n" ;
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
/*
          cout << finalJets.size ()
               << "\t" << finalLeptons.size () 
               << "\t\t" << finalLeptons.at (1).p.P() << "\t" << finalLeptons.at (1).q
               << "\t" << finalNeutrinos.size () 
               << "\n" ;
*/
        } //PG loop over events

      } //PG loop over samples 

  delete gConfigParser ;
  return 0 ;
}

