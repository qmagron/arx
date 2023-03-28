#include "queries/GenericQuery.hpp"
#include "queries/DeleteQuery.hpp"
#include "queries/InsertQuery.hpp"
#include "queries/SelectQuery.hpp"
#include "queries/UpdateQuery.hpp"
#include "string"
#include <boost/algorithm/string.hpp>
#include <iostream>

GenericQuery QueryBuilder::buildQuery(std::string query) {
  std::vector<std::string> type;
  boost::split(type, query, boost::is_any_of(" "));

  if (boost::iequals(type[0], "SELECT")) {
    return SelectQuery(query);
  } else if (boost::iequals(type[0], "UPDATE")) {
    return UpdateQuery(query);
  } else if (boost::iequals(type[0], "DELETE")) {
    return DeleteQuery(query);
  } else if (boost::iequals(type[0], "INSERT")) {
    return InsertQuery(query);
  } else {
    std::cerr << "Query could not be parsed" << std::endl;
    throw std::exception();
  }
}
