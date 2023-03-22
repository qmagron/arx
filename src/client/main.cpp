#include <string>

#include "client/Client.hpp"

constexpr unsigned int OUT_PORT = 1234;
constexpr unsigned int REMOTE_PORT = 1235;

int main() {
  Client client(OUT_PORT, REMOTE_PORT);
  client.start();

  return 0;
}
