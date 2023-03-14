#ifndef __Select_Query__
#define __Select_Query__

#include <string>
#include <vector>
#include "GenericQuery.hpp"

class SelectQuery : public GenericQuery {
private:
  std::string table;
  std::vector<std::string> columns;
  std::vector<std::string> whereClauses;
  std::string order;
  std::string limit;
  std::string offset;

public:
    SelectQuery(std::string);
    ~SelectQuery() = default;    
};




#endif