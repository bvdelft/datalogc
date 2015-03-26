#ifndef __INCLUDE_STRUCTURES
#define __INCLUDE_STRUCTURES

namespace client
{

  struct atom
  {
    std::string predicate;
    std::vector<std::string> arguments;
    std::string toString() const
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
  };

  struct clause
  {
    atom head;
    std::vector<atom> body;
  };
}

#endif
