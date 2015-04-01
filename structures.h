#ifndef __INCLUDE_STRUCTURES
#define __INCLUDE_STRUCTURES

namespace client
{

  struct atom
  {
    std::string predicate;
    std::vector<std::string> arguments;
    std::string toString() const; 
    bool operator == (const atom &a) const; 
    bool operator < (const atom &a) const; 
  };

  struct clause
  {
    atom head;
    std::vector<atom> body;
  };
}

#endif
