#ifndef __Update_Query__
#define __Update_Query__

#include "GenericQuery.hpp"
#include <map>
#include <string>
#include <vector>

class UpdateQuery : public GenericQuery {

private:
  std::map<std::string, std::string> updated_values;

  std::vector<Clause> whereClauses;
  std::vector<Clause> setClauses;
  std::vector<std::string> boolOperators;
  std::string table;

public:
  UpdateQuery(std::string, std::string);
  ~UpdateQuery(){};
  //void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>) override;
};

#endif