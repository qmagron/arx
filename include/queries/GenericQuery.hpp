
#include <string>




class GenericQuery{
 private:
  std::string type;

 public:
  GenericQuery( std::string);
  ~GenericQuery() = default;
};
