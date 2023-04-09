#ifndef __Delete_Query__
#define __Delete_Query__

#include "GenericQuery.hpp"
#include <vector>
#include "Base.hpp"

// data type to store relevant informations regarding a delete query. Does not
// allow for emebedded queries (may change in the future)
class DeleteQuery : public GenericQuery {
private:
  std::string table;
  std::vector<Clause> clauses;
  std::vector<std::string> boolOperators;

public:
  DeleteQuery(std::string, std::string);
  ~DeleteQuery(){};
  //void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>) override;
};

#endif