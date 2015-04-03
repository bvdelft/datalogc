#include <vector>
// #include <iostream> // debugging
#include <map>
#include <string>
#include <set>

#include "structures.h"

using namespace std;
using namespace client;

void debug(string msg)
{
  return;
  // cout << "DEBUG bruteforce.cpp: " << msg << endl;
}

void printdb(string name, vector<atom> db)
{
  return;
  /*
  cout << "DEBUG bruteforce.cpp: " << name << ": ";
  for (atom a : db)
    cout << a.toString() << " , ";
  cout << endl;
  */
}

/*
 * Substitute arguments in atom "a" according to substitution "sub".
 */
atom apply_sub(map<string, string> sub, atom a)
{
  atom res;
  res.predicate = a.predicate;
  vector<string> args;
  for (string s : a.arguments)
  {
    map<string,string>::const_iterator i = sub.find(s);
    if (i != sub.end())
      args.push_back(i->second);
    else
      args.push_back(s);
  }
  res.arguments = args;
  return res;
}

/*
   Find a mapping from the variables in atom "a" to the constants in "fact"
   (assuming that there are no variables in "fact"). Extends the substitution
   "sub" accordingly, returning false if no substitution could be found.
 */
bool find_sub(atom a, atom fact, map<string, string> &sub)
{
  if (a.predicate.compare(fact.predicate) != 0)
    return false;
  if (a.arguments.size() != fact.arguments.size())
    return false;
  for (unsigned int i = 0; i < a.arguments.size(); i++)
  {
    if (a.isConstant(i)) // islower(a.arguments[i][0])) // constant
    {
      if (fact.arguments[i].compare(a.arguments[i]) != 0)
        return false;
      continue;
    }
    sub[a.arguments[i]] = fact.arguments[i];
    for (unsigned int j = i+1; j < a.arguments.size(); j++)
      if (a.arguments[j].compare(a.arguments[i]) == 0)
        a.arguments[j] = fact.arguments[i];
  }
  return true;
}

/*
   Collect all substitutions that make all atoms in "body" ground to facts in
   the provided EDB.
 */
vector<map<string, string> > find_all_subs(
    vector<atom> edb, vector<atom> body)
{
  debug("find_all_subs");
  vector<map<string, string> > subs;
  map<string, string> empty;
  subs.push_back(empty);
  for (atom a : body)
  {
    vector<map<string, string> > nsubs;
    for (map<string, string> sub : subs)
    {
      atom a_ = apply_sub(sub, a);
      for (atom fact : edb)
      {
        map<string, string> s = sub;
        if (find_sub(a_, fact, s))
          nsubs.push_back(s);
      }
    }
    subs = nsubs;
  }
  return subs;
}

/*
   Return only those atoms in "db" to which "query" can be substituted.
 */
vector<atom> filter_match(atom query, vector<atom> db)
{
  debug("filter_match");
  printdb("db",db);
  vector<atom> result;
  debug("query: " + query.toString());
  map<string, string> sub;
  for (atom a : db) {
    if (find_sub(query, a, sub)) {
      debug("yes: " + a.toString());
      result.push_back(a);
    } else {
      debug("no: " + a.toString());
    }
  }
  printdb("result",result);
  return result;
}

/*
   One round of deriving all possible facts from "edb" using each rule in
   "program".
 */
vector<atom> derive_iter(vector<atom> edb, vector<clause> program)
{
  debug("derive_iter");
  vector<atom> idb = edb;
  for (clause c : program)
  {
    vector<map<string, string> >subs = find_all_subs(edb, c.body);
    for (map<string, string> s : subs)
      idb.push_back(apply_sub(s, c.head));
  }
  return idb;
}

/*
   Return a vector with the same elements in "original", except those that
   occur in remove.
 */
vector<atom> remove_all(vector<atom> original, vector<atom> remove)
{
  vector<atom> result = original;
  debug("remove_all ; removing " + to_string(remove.size()) + " from " + to_string(original.size()) );
  if (original.size() == 0)
    return original;
  for (const atom a : remove)
  {
    auto filter = [a](atom b) { return a.predicate.compare(b.predicate) == 0 &&
      a.arguments.size() == b.arguments.size(); };
    for (vector<atom>::iterator it = result.begin();
        it != result.end();)
      if (a.predicate.compare(it->predicate) == 0 &&
          a.arguments.size() == it->arguments.size() &&
          equal(a.arguments.begin(), a.arguments.end(), it->arguments.begin()))
        it = result.erase(it);
      else
        it++;
  }
  return result;
}

/*
   Call derive_iter, extend the db until no new facts are derived. Return
   total set of facts (initial + derived).
 */
vector<atom> derive_all(vector<atom> edb, vector<clause> program)
{
  bool new_facts;
  do
  {
    debug("Looping derive all");
    new_facts = false;
    printdb("edb",edb);
    vector<atom> idb = derive_iter(edb, program);
    printdb("idb",idb);
    vector<atom> res = remove_all(idb, edb);
    printdb("res",res);
    debug("Returned from remove_all");
    debug("Size: " + to_string(res.size()));
    new_facts = !(res.empty());
    debug("New facts?");
    debug(new_facts ? "yes" : "no");

    // remove duplicates
    set<atom> s(idb.begin(), idb.end());
    vector<atom> v(s.begin(), s.end());
    edb = v;
  } while (new_facts);
  return edb;
}

/*
   Bruteforce derive all the facts from the given "edb" and "program". Return
   those facts that match the query.
 */
vector<atom> bruteforce(atom query, vector<atom> edb, vector<clause> program)
{
  debug("Entered bruteforce()");
  vector<atom> idb = derive_all(edb, program);
  debug("Starting filter");
  vector<atom> matches = filter_match(query, idb);
  debug("Returning bruteforce()");
  return matches;
}
