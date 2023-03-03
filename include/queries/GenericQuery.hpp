
#include <string>




class GenericQuery{
 private:
  std::string type;

 public:
  static GenericQuery( std::string) noexcept;
  ~GenericQuery() = default;
};
