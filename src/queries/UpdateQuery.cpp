#include "queries/UpdateQuery.hpp"
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.hpp>
#include "Base.hpp"

UpdateQuery::UpdateQuery(std::string query, std::string table) {
  this->table = table;
  std::vector<std::string> temp = std::vector<std::string>();

  std::string copy;

  whereClauses = std::vector<Clause>();
  setClauses = std::vector<Clause>();
  boolOperators = std::vector<std::string>();
  boost::regex expr{"(?i)\\s*UPDATE\\s+([a-zA-Z0-9_]+)\\s+SET\\s+([a-zA-Z0-9_]+"
                    "\\s*=\\s*[^,]+)(?:\\s*,\\s*([a-zA-Z0-9_]+\\s*=\\s*[^,]+))*"
                    "\\s*(?:WHERE\\s+([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\"[a-zA-z0-9]"
                    "+\"|[0-9]+))?\\s*;\\s*"};
  boost::regex whereExpr{
      "([a-zA-Z]+)\\s*(=|<=|>=)\\s*(\'[a-zA-z0-9]+\'|[0-9]+)"};
  boost::regex setExpr{"([a-zA-Z]+)\\s*(=)\\s*(\'[a-zA-z0-9]+\'|[0-9]+)"};
  boost::regex boolExpr{"(?i)(and|or)(?-i)"};
  boost::smatch caughtClauses;
  boost::smatch boolOp;



  if (boost::regex_match(query, expr)) {
    boost::algorithm::split_regex(temp, query,
                                  boost::regex{"(?i)(SET | where)"});

    copy = temp[2];
    while (boost::regex_search(temp[1], caughtClauses, setExpr)) {

      boost::split(temp,caughtClauses[0],boost::is_any_of("="));
      setClauses.push_back(Clause(temp[0], "=", temp[1]));
      caughtClauses.suffix().str();
    }
    
    

    while (boost::regex_search(temp[2], caughtClauses, whereExpr)) {
      whereClauses.push_back(
          Clause(caughtClauses[1], caughtClauses[2], caughtClauses[3]));
      temp[2] = caughtClauses.suffix().str();
    }

    while (boost::regex_search(copy, boolOp, boolExpr)) {
      boolOperators.push_back(boolOp[1]);
      copy = boolOp.suffix().str();
    }

  } else {
    std::cerr << "Syntax error on Update Query" << std::endl;
    throw std::exception();
  }
}



void UpdateQuery::encryptQuery(
    std::map<std::string, std::map<std::string, int>> counters,
    std::map<std::string, std::map<std::string, byte[16]>> field_keys
    ,byte fieldEncryptionKey[16]) {
  byte counter[4] = {0, 0, 0, 0};
  table = Base::encryptBASE(table, counter,fieldEncryptionKey);
}