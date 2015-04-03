#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <complex>
#include <ctype.h>

#include <fstream>
#include <streambuf>

#include "parser.h"
#include "structures.h"
#include "bruteforce.h"

using namespace std;
using namespace client;

template<typename T> 
vector<T> filter(vector<T> v, bool (*f)(T))
{
  vector<T> result;
  for (typename vector<T>::const_iterator i = v.begin(); i != v.end(); ++i)
  {
    if ((*f)(*i))
      result.push_back(*i);
  }
  return result;
}

bool isFact(client::clause c) {
  return c.body.empty();
}

bool isRule(client::clause c) {
  return !c.body.empty();
}

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "Expecting argument: inputfile" << endl << endl;
    return -1;
  }

  ifstream t(argv[1]);
  if (!t.is_open()) {
    cout << "Could not open file " << argv[1] << endl;
    return -1;
  }
  string input_data ((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

  vector<client::clause> clauses;
  cout << "Parsing..." << endl;
  bool r = client::parse(input_data, clauses);
  if (!r)
  {
    cout << "Parsing failed." << endl;
    return -2;
  }
  cout << "Parsing succeeded." << endl;

  vector<client::clause> facts_ = filter(clauses, &isFact);
  vector<client::atom> facts;
  for (vector<client::clause>::const_iterator i = facts_.begin(); i != facts_.end(); ++i)
  {
    facts.push_back(i->head);
  }
  vector<client::clause> rules = filter(clauses, &isRule);

  cout << "Facts: " << facts.size() << endl;
  cout << "Rules: " << rules.size() << endl;
  
  //derive_all(facts, rules);
  
  //cout << "Derived facts: " << facts.size() << endl;

    while(true)
  {
    cout << "Query > ";
    string str;
    getline(std::cin, str);
    if (str.empty())
      break;
    vector<client::clause> queries;
    bool ok = client::parse(str, queries);
    if (!ok || queries.size() != 1) {
      cout << "Not parseable." << endl;
      continue;
    }
    cout << "Result set:" << endl;
    client::clause query = queries[0];
    vector<atom> res = bruteforce(query.head, facts, rules);
    for (atom a : res)
      cout << a.toString() << endl;
  }


  return 0;
}
