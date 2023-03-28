#include <string>

#include "client-proxy/ClientProxy.hpp"
#include "queries/GenericQuery.hpp"


constexpr unsigned int IN_PORT = 1235;
constexpr unsigned int OUT_PORT = 1236;
constexpr unsigned int REMOTE_PORT = 1237;


int main() {
  ClientProxy client(IN_PORT, OUT_PORT);  
  QueryBuilder::buildQuery("Delete from caca where zizi=1 and zizi=0;");

  //client.start(REMOTE_PORT);

  return 0;
}
