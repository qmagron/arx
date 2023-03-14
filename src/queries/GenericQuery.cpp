#include "queries/GenericQuery.hpp"
#include "string"

GenericQuery::GenericQuery(std::string query){
       type = query;
}

GenericQuery QueryBuilder::buildQuery(std::string query){
    return GenericQuery(query);
}

