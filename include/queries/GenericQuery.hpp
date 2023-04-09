#ifndef __Build_Query__
#define __Build_Query__

#include <string>
#include <iostream>

// struct that contains the data about a clause of a query
struct Clause {
  std::string column;
  std::string op;
  std::string value;

  Clause(std::string column, std::string op, std::string value);
  friend std::ostream& operator<<(std::ostream& os, const Clause& clause);
};



// generic query virtual class to allow polymorphism at building then dispatches
// the query to the relevant query type
class GenericQuery {
public:
  ~GenericQuery() = default;
  //virtual void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>) = 0;
};

namespace QueryBuilder {
GenericQuery* buildQuery(std::string);
}

#endif