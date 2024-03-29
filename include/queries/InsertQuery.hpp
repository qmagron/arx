#ifndef __Insert_Query__
#define __Insert_Query__

#include "GenericQuery.hpp"
#include <string>
#include <vector>
#include <cryptopp/aes.h>


#ifdef __CRYPTOPP_BYTE__
using byte = CryptoPP::byte;
#endif


// class that heriates from GenericQuery
class InsertQuery : public GenericQuery {
private:
  std::string table;
  std::vector<std::string> fields;
  std::vector<std::string> values;



public:
  InsertQuery(std::string, std::string);
  ~InsertQuery(){};
  void encryptQuery(std::map<std::string, std::map<std::string, int>>,std::map<std::string, std::map<std::string, byte[16]>>,
                   byte[16]) override;
};
#endif
