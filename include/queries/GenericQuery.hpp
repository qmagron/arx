#ifndef __Build_Query__
#define __Build_Query__


#include <string>


class GenericQuery{
 private:
  std::string type;

 public:
  GenericQuery( std::string);
  ~GenericQuery() = default;
};


namespace QueryBuilder{
    GenericQuery buildQuery(std::string);
}

#endif