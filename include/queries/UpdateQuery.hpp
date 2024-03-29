#ifndef __Update_Query__
#define __Update_Query__

#include "GenericQuery.hpp"
#include <map>
#include <string>
#include <vector>
#include <cryptopp/aes.h>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


class UpdateQuery : public GenericQuery {

private:
  std::map<std::string, std::string> updated_values;

  std::vector<Clause> whereClauses;
  std::vector<Clause> setClauses;
  std::vector<std::string> boolOperators;
  std::string table;

  bool checkOperatorsValidity() const;

public:
  UpdateQuery(std::string, std::string);
  ~UpdateQuery(){};
  void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>,
                   byte[16]) override;
};

#endif
