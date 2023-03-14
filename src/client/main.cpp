#include "client/ClientProxy.hpp"


constexpr unsigned int IN_PORT = 1234;
constexpr unsigned int OUT_PORT = 1235;
constexpr unsigned int REMOTE_PORT = 1236;
const std::string REMOTE = "127.0.0.1";


int main() {
  ClientProxy client(IN_PORT, OUT_PORT);
  client.start(REMOTE, REMOTE_PORT);

  return 0;
}
