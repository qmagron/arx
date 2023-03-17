#include "server-proxy/ServerProxy.hpp"


constexpr unsigned int IN_PORT = 1236;
constexpr unsigned int OUT_PORT = 1237;
constexpr unsigned int REMOTE_PORT = 5555;  // TODO change for DBMS
const std::string REMOTE = "127.0.0.1";


int main() {
  ServerProxy server(IN_PORT, OUT_PORT);
  server.start(REMOTE, REMOTE_PORT);

  return 0;
}
