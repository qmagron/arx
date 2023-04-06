#ifndef __Select_Query__
#define __Select_Query__

#include <string>
#include <vector>
#include "GenericQuery.hpp"

class SelectQuery : public GenericQuery {
private:
    std::string table;
    std::vector<whereClause> clauses;
    std::vector<std::string> boolOperators;

public:
    SelectQuery(std::string,std::string);
    ~SelectQuery() = default;    
};




#endif