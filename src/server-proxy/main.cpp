#include "server-proxy/ServerProxy.hpp"


constexpr unsigned int IN_PORT = 1237;
constexpr unsigned int REMOTE_PORT = 5432;


int main() {
  ServerProxy server(IN_PORT);
  server.start(REMOTE_PORT);

  return 0;
}
