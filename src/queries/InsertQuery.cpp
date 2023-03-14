#include "queries/InsertQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>

InsertQuery::InsertQuery(std::string query) {
        std::vector<std::string> tokens;
        boost::split(tokens, query, boost::is_any_of(" "));


}