#ifndef __Delete_Query__
#define __Delete_Query__

#include "GenericQuery.hpp"
#include <vector>


// data type to store relevant informations regarding a delete query. Does not allow for emebedded queries (may change in the future)
class DeleteQuery : public GenericQuery {
private:
    std::string table;
    std::vector<whereClause> clauses;
    std::vector<std::string> boolOperators;

public:
    DeleteQuery(std::string,std::string);
    ~DeleteQuery() = default;
};


#endif