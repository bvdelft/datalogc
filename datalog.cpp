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

/*
// Assumes length of from and to is equal.
// To is assumed to be a fact (no variables).
bool substitute(map<string,string> s, vector<string> from, vector<string> to)
{
  if (from.size() == 0)
    return true;
  for (unsigned int i = 0; i < from.size(); ++i)
  {
    if (islower(from[i][0]))
    { // Both constants.
      if (from[i].compare(to[i]) == 0)
        continue;
      else
        return false;
    }
    else
    { // Map variable.
      s[from[i]] = to[i];
      // update any variable in from accordingly
      for (unsigned int j = i+1; j < from.size(); ++j)
        if (from[j].compare(from[i]) == 0)
          from[j] = to[i];
    }
  } 
  return true;
}


void derive_all(vector<atom> &facts, vector<clause> rules)
{
  bool new_facts = false;
  do
  {
    new_facts = false;
    for (vector<clause>::const_iterator i = rules.begin();
        i < rules.end(); ++i)
    {
      vector<map<string, string> > subs;
      map<string, string> empty;
      subs.push_back(empty);
      for (vector<atom>::const_iterator a = (*i).body.begin();
          a < (*i).body.end(); ++a)
      { // For each atom in the body
        vector<map<string, string> > extended;
        for (vector<map<string, string> >::const_iterator s = subs.begin();
            s < subs.end(); ++s)
        { // Preform each substitution so far
          string pred = a->predicate;
          vector<string> args = a->arguments;
          for (unsigned int x = 0; x < args.size(); ++x)
          { 
            map<string,string>::const_iterator it = s->find(args[x]);
            if (it != s->end())
              args[x] = it->second;
          }
          for (vector<atom>::const_iterator f = facts.begin();
              f < facts.end(); ++f)
          {
            if (pred.compare(f->predicate) != 0)
              continue;
            map<string,string> sub = *s;
            if (!substitute(sub, args, f->arguments))
              continue;
            extended.push_back(sub);
            new_facts = true;
          }
        }
        cout << "Extended length> " << extended.size() << endl;
        subs = extended; // only leave extended substitutions.
      }
      // apply on the head, add new ones.
      for (vector<map<string, string> >::const_iterator s = subs.begin();
          s < subs.end(); ++s)
      {
        vector<string> copy = i->head.arguments;
        for(unsigned int x = 0; x < copy.size(); ++x) {
          map<string,string>::const_iterator it = s->find(copy[x]);
          if (it != s->end())
            copy[x] = it->second;
        }
        atom res;
        res.predicate = i->head.predicate;
        res.arguments = copy;
        facts.push_back(res);
        cout << "Added fact: " << res.toString() << endl;
      }
    }
    break;
  } while (new_facts);
}

bool substitute(map<string,string> res, atom from, atom to)
{
  if (from.predicate.compare(to.predicate) != 0)
    return false;
  if (from.arguments.size() != to.arguments.size())
    return false;
  return substitute(res, from.arguments, to.arguments);
}
bool canSubstitute(atom a, atom b)
{
  map<string,string> res;
  return substitute(res,a,b);
}
*/
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
