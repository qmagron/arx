#include "queries/DeleteQuery.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <vector>

DeleteQuery::DeleteQuery(std::string query, std::string table)
  : GenericQuery() {
  this->table = table;
  std::string queryCopy = query;
  clauses = std::vector<Clause>();
  boolOperators = std::vector<std::string>();
  // regex to check wether or not the query matches the delete syntax
  boost::regex expr{"(?i)delete\\s+from\\s+(?-i)[a-zA-Z]+(((?i)\\s*where\\s*(?-"
    "i)[a-zA-Z]+(=|<=|>=)(\'[a-zA-z0-9]+\'|[0-9]+)+)\\s*(((?i)"
    "and|or(?-i))\\s*[a-zA-Z]+(=|<=|>=)[a-zA-z0-9]+)*)?\\s*;"};
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

  }
  else {
    std::cerr << "Syntax error on Delete Query" << std::endl;
    throw std::exception();
  }
}

void DeleteQuery::encryptQuery(std::map<std::string, std::map<std::string, int>> counters, std::map<std::string, std::map<std::string, byte[16]>> field_keys, byte fieldEncryptionKey[16]) {

  byte counter[4] = { 0, 0, 0, 0 };
  table = Base::encryptBASE(table, counter, fieldEncryptionKey);

  if (!checkOperatorsValidity()) {
    std::cerr << "Encrypted fields cannot have multiple similar range operators attached to them" << std::endl;
    throw std::exception();
    // TODO handle this better
  }

  

}


bool DeleteQuery::checkOperatorsValidity() const {
  std::vector<std::pair<std::string, std::string>> encryptedFieldWithRangeOperator =
    std::vector<std::pair<std::string, std::string>>();

  for (auto& clause : clauses) {
    for (auto& field_op : encryptedFieldWithRangeOperator) {
      if (boost::iequals(clause.column, field_op.first) && boost::iequals(clause.op, field_op.second)) {
        return false;
      }
    }
    encryptedFieldWithRangeOperator.push_back(std::pair<std::string, std::string>(clause.column, clause.op));
  }
  return true;
}