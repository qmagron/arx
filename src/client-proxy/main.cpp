#include <string>
#include <iostream>

#include "client-proxy/ClientProxy.hpp"
#include "queries/GenericQuery.hpp"
#include "queries/DeleteQuery.hpp"
#include "Base.hpp"


constexpr unsigned int IN_PORT = 1235;
constexpr unsigned int OUT_PORT = 1236;
constexpr unsigned int REMOTE_PORT = 1237;


int main() {

  ClientProxy client(IN_PORT, OUT_PORT);
  client.start(REMOTE_PORT);


  return 0;
}
