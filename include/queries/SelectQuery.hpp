#ifndef __Select_Query__
#define __Select_Query__

#include "GenericQuery.hpp"
#include <string>
#include <vector>
#include <cryptopp/aes.h>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


class SelectQuery : public GenericQuery {
private:
  std::string table;
  std::vector<Clause> clauses;
  std::vector<std::string> boolOperators;

  bool checkOperatorsValidity() const;
public:
  SelectQuery(std::string, std::string);
  ~SelectQuery(){};
 void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>,
                   byte[16]) override;
};

#endif
