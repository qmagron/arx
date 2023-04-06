#include "queries/InsertQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>

InsertQuery::InsertQuery(std::string query, std::string table) {
  this->table = table;
  std::string queryCopy = query;
  fields = std::vector<std::string>();
  values = std::vector<std::string>();

  std::vector<std::string> temp = std::vector<std::string>();

  boost::regex exprWithFields{
      "(?i)\\s*insert\\s+into\\s*[a-zA-Z]+\\s*(\\(.+(,.)*\\))"
      "\\s+values(\\s*\\(.+(,.+)\\))+\\s*;"};

  boost::regex exprWithoutFields{"(?i)\\s*insert\\s+into\\s*[a-zA-Z]+"
                                 "\\s+values(\\s*\\(.+(,.+)\\))+\\s*;"};

  boost::smatch caughtFields;
  boost::smatch caughtValues;
  boost::regex valuesExpr{"\\((([a-zA-Z]+|\".\")(,([a-zA-Z]+|\".\")+)*)\\)"};
  boost::regex fieldsExpr{"\\(([a-zA-Z]+(,[a-zA-Z]+)*)\\)"};

  std::string valuesString;

  if (boost::regex_match(query, exprWithFields)) {
    if (boost::regex_search(query, caughtFields, fieldsExpr)) {
      std::string fieldsString = caughtFields[1];
      boost::split(fields, fieldsString, boost::is_any_of(","));
      query = caughtFields.suffix().str();
    }
    
    while (boost::regex_search(query, caughtValues, valuesExpr)) {
      valuesString = caughtValues[1];

      boost::split(temp, valuesString, boost::is_any_of(","));
      values.insert(values.end(), temp.begin(), temp.end());
      temp.clear();
      query = caughtValues.suffix().str();
    }

  } else if (boost::regex_match(query, exprWithoutFields)) {
    
    while (boost::regex_search(query, caughtValues, valuesExpr)) {
      valuesString = caughtValues[1];

      boost::split(temp, valuesString, boost::is_any_of(","));
      values.insert(values.end(), temp.begin(), temp.end());
      temp.clear();
      query = caughtValues.suffix().str();
    }
  } else {
    std::cerr << "Syntax error on Update Query" << std::endl;
    throw std::exception();
  }
  

}