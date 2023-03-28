#ifndef __Build_Query__
#define __Build_Query__

#include <string>

// struct that contains the data about the where clause of a query
struct whereClause {
  std::string column;
  std::string op;
  std::string value;
};


// generic query virtual class to allow polymorphism at building then dispatches the query to the relevant query type
class GenericQuery {
public:
  GenericQuery() = default;
  ~GenericQuery() = default;
};

namespace QueryBuilder {
GenericQuery buildQuery(std::string);
}

#endif