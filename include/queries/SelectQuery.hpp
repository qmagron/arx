#ifndef __Select_Query__
#define __Select_Query__

#include "GenericQuery.hpp"
#include <string>
#include <vector>

class SelectQuery : public GenericQuery {
private:
  std::string table;
  std::vector<Clause> clauses;
  std::vector<std::string> boolOperators;

public:
  SelectQuery(std::string, std::string);
  ~SelectQuery() = default;
 void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>) override;
};

#endif