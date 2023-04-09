#include "queries/GenericQuery.hpp"
#include "queries/DeleteQuery.hpp"
#include "queries/InsertQuery.hpp"
#include "queries/SelectQuery.hpp"
#include "queries/UpdateQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>
#include <iostream>

Clause::Clause(std::string column, std::string op, std::string value) {
  this->column = boost::trim_copy(column);
  this->op = boost::trim_copy(op);
  this->value = boost::trim_copy(value);
}
std::ostream& operator<<(std::ostream& os, const Clause& cl)
{
    os << cl.column << ' ' << cl.op << ' ' << cl.value << std::endl;
    return os;
}

GenericQuery* QueryBuilder::buildQuery(std::string query) {
  std::vector<std::string> type;
  boost::trim(query);
  boost::split(type, query, boost::is_any_of(" "));

  if (boost::iequals(type[0], "SELECT")) {
    return new SelectQuery(query, type[2]);
  } else if (boost::iequals(type[0], "UPDATE")) {
    return new UpdateQuery(query, type[2]);
  } else if (boost::iequals(type[0], "DELETE")) {
    return new DeleteQuery(query, type[2]);
  } else if (boost::iequals(type[0], "INSERT")) {
    return new InsertQuery(query, type[2]);
  } else {
    std::cerr << "Query could not be parsed" << std::endl;
    throw std::exception();
  }
}