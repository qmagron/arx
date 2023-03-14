#include "queries/SelectQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>

// parse the query and store its elements in the class with the constructor
SelectQuery::SelectQuery(std::string query) {
  std::vector<std::string> tokens;
  boost::split(tokens, query, boost::is_any_of(" "));
  std::vector<std::string> columns;
  boost::split(columns, tokens[1], boost::is_any_of(","));
  for (int i = 0; i != columns.size(); ++i) {
    this->columns.push_back(columns[i]);
  }
  this->table = tokens[3];
  std::vector<std::string> where;
  boost::split(where, tokens[5], boost::is_any_of("="));
  
}
