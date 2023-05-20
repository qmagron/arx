#include "queries/SelectQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "Base.hpp"

// parse the query and store its elements in the class with the constructor
SelectQuery::SelectQuery(std::string query, std::string table) {
  this->table = table;
  std::string queryCopy = query;
  clauses = std::vector<Clause>();
  boolOperators = std::vector<std::string>();
  // regex to check wether or not the query matches the delete syntax
  boost::regex expr{
      "(?i)select\\s+(([a-zA-Z]+(,[a-zA-Z]+)*)|\\*) "
      "from\\s+[a-zA-Z]+((\\s*where\\s*[a-zA-Z]+(=|<=|>=)(\'[a-zA-z0-9]+\'|[0-"
      "9]+)"
      "+)\\s*((and|or)\\s*[a-zA-Z]+(=|<=|>=)[a-zA-z0-9]+)*)?\\s*;"};
  // expression to catch relevant fields from the query
  boost::regex whereExpr{
      "([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\'[a-zA-z0-9]+\'|[0-9]+)"};
  // expression to catch boolean operators
  boost::regex boolExpr{"(?i)(and|or)(?-i)"};
  boost::smatch caughtClauses;
  boost::smatch boolOp;

  if (boost::regex_match(query, expr)) {
    while (boost::regex_search(query, caughtClauses, whereExpr)) {
      clauses.push_back(
          Clause(caughtClauses[1], caughtClauses[2], caughtClauses[3]));
      query = caughtClauses.suffix().str();
    }

    while (boost::regex_search(queryCopy, boolOp, boolExpr)) {
      boolOperators.push_back(boolOp[1]);
      queryCopy = boolOp.suffix().str();
    }

  } else {
    std::cerr << "Syntax error on Select Query" << std::endl;
    throw std::exception();
  }
}



void SelectQuery::encryptQuery(
    std::map<std::string, std::map<std::string, int>> counters,
    std::map<std::string, std::map<std::string, byte[16]>> field_keys
    ,byte fieldEncryptionKey[16]) {
  byte counter[4] = {0, 0, 0, 0};
  // table = Base::encryptBASE(table, counter,fieldEncryptionKey);
}
