#include "queries/UpdateQuery.hpp"
#include <string>
#include <vector>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

UpdateQuery::UpdateQuery(std::string query, std::string table) {
  this->table = table;
  std::vector<std::string> temp;

  clauses = std::vector<whereClause>();
  boolOperators = std::vector<std::string>();
  boost::regex expr{"(?i)\\s*UPDATE\\s+([a-zA-Z0-9_]+)\\s+SET\\s+([a-zA-Z0-9_]+"
                    "\\s*=\\s*[^,]+)(?:\\s*,\\s*([a-zA-Z0-9_]+\\s*=\\s*[^,]+))*"
                    "\\s*(?:WHERE\\s+([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\"[a-zA-z0-9]+\"|[0-9]+))?\\s*;\\s*"};
  boost::regex whereExpr{
      "([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\"[a-zA-z0-9]+\"|[0-9]+)"};
  boost::regex boolExpr{"(?i)(and|or)(?-i)"};
  boost::smatch caughtClauses;
  boost::smatch boolOp;

  if (boost::regex_match(query, expr)) {
    boost::algorithm::split_regex( temp, query, boost::regex{ "(?i)(SET | where)"} ) ;
    for (auto i: temp) {
      std::cout << i << std::endl;
    }
  }
  else {
    std::cerr << "Syntax error on Update Query" << std::endl;
    throw std::exception();
  }
}
