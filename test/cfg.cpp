#include "CfgParser.h"

using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  string sTest          = gConfigParser->readStringOpt ("field::stringa") ;
  int iTest             = gConfigParser->readIntOpt ("field::intero") ;
  float fTest           = gConfigParser->readFloatOpt ("field::reale") ;
  vector<int> ivTest    = gConfigParser->readIntListOpt ("field::intlist") ;
  vector<float> fvTest  = gConfigParser->readFloatListOpt ("field::floatlist") ;
  vector<string> svTest = gConfigParser->readStringListOpt ("field::stringlist") ;

  cout << "string\t" << sTest << endl ;
  cout << "int\t"    << iTest << endl ;
  cout << "float\t"  << fTest << endl ;
  cout << endl ;
  for (unsigned int i = 0 ; i < ivTest.size () ; ++i)
    cout << ivTest.at (i) << " " ;
    
  cout << endl ;
  for (unsigned int i = 0 ; i < fvTest.size () ; ++i)
    cout << fvTest.at (i) << " " ;
    
  cout << endl ;
  for (unsigned int i = 0 ; i < svTest.size () ; ++i)
    cout << svTest.at (i) << " " ;
  cout << endl ;
 
  delete gConfigParser ;

  return 0 ;
}