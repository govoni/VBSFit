// http://www.cplusplus.com/reference/string/string/getline/

// extract to string
#include <iostream>
#include <string>

using namespace std ;

int main ()
{
  string name;

  cout << "Please, enter your full name: " ;
//  cout << static_cast<bool> (getline (std::cin, name) ) << endl ;
  cout << getline (std::cin, name).good () << endl ;
  cout << "Hello, " << name << "!\n" ;

  return 0;
}