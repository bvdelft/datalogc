#include <string>
#include <vector>
#include <ctype.h>

#include "structures.h"

namespace client
{

  using namespace std;

  bool atom::isVariable(int i) const
  {
    return isupper(arguments[i][0]);
  }

  bool atom::isConstant(int i) const
  {
    return !isVariable(i);
  }

  string atom::toString () const
  {
      std::string res = predicate + "(";
      for (std::vector<std::string>::const_iterator i = arguments.begin();
          i != arguments.end();)
      {
        res = res + (*i);
        if (++i != arguments.end())
          res = res + ",";
      }
      res += ")";
      return res;
    }

  bool atom::operator == (const atom &a) const
  {
    return (predicate.compare(a.predicate) == 0
      && (arguments.size() == a.arguments.size())
      && std::equal(arguments.begin(), arguments.end(), a.arguments.begin()));
  }

  bool atom::operator < (const atom &a) const
  {
    if (predicate.compare(a.predicate) < 0 
        || (arguments.size() < a.arguments.size()))
      return true;
    for (unsigned int i = 0; i < arguments.size(); i++)
      if (arguments[i].compare(a.arguments[i]) < 0)
        return true;
    return false;
  }

}

