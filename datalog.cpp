#include <iostream>
#include <vector>
#include <string>
#include <complex>

#include <fstream>
#include <streambuf>

#include "parser.h"
#include "structures.h"

int main(int argc, char ** argv)
{
  using namespace std;
  cout << "Testing clause parser" << endl;
  cout << "Type q to quite" << endl << endl;
  ifstream t(argv[1]);
  string str((istreambuf_iterator<char>(t)),
      istreambuf_iterator<char>());

  vector<client::clause> c;
  cout << "Parsing: " << str;
  bool r = client::parse(str, c);
  if (r)
  {
    cout << "Parsing succeeded" << endl;
    for (vector<client::clause>::const_iterator i = c.begin();
        i != c.end(); ++i)
    {
      cout << i->head.predicate << endl;
    }
  }
  else
  {
    cout << "Parsing failed" << endl;
  }
  cout << "Bye :-)" << endl;
  return 0;
}
