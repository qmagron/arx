#ifndef __ARX_EQ__
#define __ARX_EQ__

#include "queries/GenericQuery.hpp"
#include <vector>
#include "Base.hpp"

class SearchToken {
private:
  std::vector<Cipher<32>> possibleCiphers;
  bool empty;

public:
  SearchToken(unsigned value, unsigned counter, byte[16]);
  std::vector<Cipher<32>> ApplyToken(std::vector<Cipher<32>> database);
};

std::vector<unsigned> searchArxEq(unsigned value, unsigned counter, std::vector<Cipher<32>> database, byte key[16]);


#endif