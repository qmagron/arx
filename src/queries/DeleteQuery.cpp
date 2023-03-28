#include "queries/DeleteQuery.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <vector>

DeleteQuery::DeleteQuery(std::string query) : GenericQuery() {
  std::string  queryCopy = query;
  std::vector<whereClause> clausesVector = std::vector<whereClause>();
  std::vector<std::string> booleanOperators = std::vector<std::string>();
  // regex to check wether or not the query matches the delete syntax
  boost::regex expr{"(?i)delete\\s+from\\s+(?-i)[a-zA-Z]+(((?i)\\s*where\\s*(?-"
                    "i)[a-zA-Z]+(=|<=|>=)(\"[a-zA-z0-9]+\"|[0-9]+)+)\\s*(((?i)"
                    "and|or(?-i))\\s*[a-zA-Z]+(=|<=|>=)[a-zA-z0-9]+)*)?\\s*;"};
  // expression to catch relevant fields from the query
  boost::regex whereExpr{
      "([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\"[a-zA-z0-9]+\"|[0-9]+)"};
  // expression to catch boolean operators
  boost::regex boolExpr{"(?i)(and|or)(?-i)"};
  boost::smatch clauses;
  boost::smatch boolOp;
  if (boost::regex_match(query, expr)) {
    while (boost::regex_search(query, clauses, whereExpr)) {
      clausesVector.push_back(whereClause{clauses[1], clauses[2], clauses[3]});
      query = clauses.suffix().str();
    }

    while (boost::regex_search(queryCopy, boolOp, boolExpr)) {
      booleanOperators.push_back(boolOp[1]);
      queryCopy = boolOp.suffix().str();
    }
    

  } else {
    std::cerr << "Syntax error on Delete Query" << std::endl;
    throw std::exception();
  }

  // TODO assign the vectors to attributes
}