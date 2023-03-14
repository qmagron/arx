#ifndef __Build_Query__
#define __Build_Query__

#include <string>

class GenericQuery {
private:
  

public:
  GenericQuery() = default;
  ~GenericQuery() = default;
};

namespace QueryBuilder {
GenericQuery buildQuery(std::string);
}

#endif