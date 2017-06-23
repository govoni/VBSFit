/**
read a config file searching for selections.
*/

#include "CfgParser.h"
#include "TString.h"


using namespace std ;


int main (int argc, char **argv) 
{
  // check number of inpt parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  vector<string> selectionsListName = gConfigParser->readStringListOpt ("general", "selectionsList") ;
  cout << "selectionsList : " ;
  for (unsigned int i = 0 ; i < selectionsListName.size () ; ++i)
    cout << selectionsListName.at (i) << " " ;

  vector<TString> selectionsList ;
  cout << "\n" ;
  for (unsigned int i = 0 ; i < selectionsListName.size () ; ++i)
    {  
       selectionsList.push_back (gConfigParser->readStringOpt ("selections", selectionsListName.at (i)).c_str ()) ;
       cout << selectionsListName.at (i) << " : " << selectionsList.back () << "\n" ;
  	}


  return 0 ;
}
