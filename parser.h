#ifndef __INCLUDE_PARSER
#define __INCLUDE_PARSER

#include <vector>

#include "structures.h"

namespace client
{
  bool parse(std::string str, std::vector<clause>& clauses);
}

#endif
