#include "queries/GenericQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>
#include "queries/SelectQuery.hpp"


GenericQuery QueryBuilder::buildQuery(std::string query){
    std::vector<std::string> type;
    boost::split(type, query, boost::is_any_of(" "));

    if (boost::iequals(type[0], "SELECT"))
    {
        return SelectQuery(query);
    }
    return GenericQuery();
}

