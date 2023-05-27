#include "server-proxy/indexes/ArxEq.hpp"
#include "Base.hpp"


SearchToken::SearchToken(int value,size_t counter, byte key[16]){
  if (empty = (counter == 0))
  {
    return;
  }
    
  for (size_t i = 0; i < counter; i++)
  {
    possibleCiphers.push_back(Base::encryptInt(value,i,key));
  }
  

}

std::vector<Cipher<32>> SearchToken::ApplyToken(std::vector<Cipher<32>> database){
  std::vector<Cipher<32>> result = std::vector<Cipher<32>>();
  if (empty) return result;
  for (auto &row: database){
    for (auto &v: possibleCiphers)
    {
      if (v == row)
      {
        result.push_back(row);
        break;
      }
      
    }
    

  }
  return result;


}

std::vector<int> searchArxEq(int value,size_t counter,std::vector<Cipher<32>> database,byte key[16]){

  std::vector<int> result = std::vector<int>();
  SearchToken st = SearchToken(value,counter,key);
  auto res = st.ApplyToken(database);
  for (auto &v: res)
  {
    auto temp = Base::decryptInt(v,counter,key);
    if (temp == value)
    {
      result.push_back(temp);
    }
  }
  return result;
  
}

