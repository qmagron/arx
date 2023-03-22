#ifndef __Insert_Query__
#define __Insert_Query__

#include "GenericQuery.hpp"
#include <string>
#include <vector>

// class that heriates from GenericQuery
class InsertQuery : public GenericQuery {
private:
  std::string table;
  std::vector<std::string> columns;
  std::vector<std::string> values;

public:
  InsertQuery(std::string);
  ~InsertQuery() = default;
};
#endif