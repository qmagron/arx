#ifndef __Update_Query__
#define __Update_Query__

#include "GenericQuery.hpp"
#include <string>
#include <vector>
#include <map>

class UpdateQuery : public GenericQuery {

private:
    std::map<std::string, std::string> updated_values;

    std::vector<whereClause> clauses;
    std::vector<std::string> boolOperators;
    std::string table;

public:
    UpdateQuery(std::string,std::string);
    ~UpdateQuery() = default;
};

#endif