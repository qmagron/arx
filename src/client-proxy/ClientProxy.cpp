#include "client-proxy/ClientProxy.hpp"

#include "boost/algorithm/string.hpp"
#include <fstream>
#include <iostream>
#include <stdio.h>

ClientProxy::ClientProxy(unsigned short inPort, unsigned short outPort)
    : Proxy(inPort, outPort) {
  value_counters = new std::map<std::string, std::map<std::string, int>>();
  std::fstream file;

  file.open("counters.csv", std::fstream::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
  }

  std::string line;
  std::vector<std::string> tokens;
  while (std::getline(file, line)) {
    boost::split(tokens, line, boost::is_any_of(","));
    if (value_counters->find(tokens[0]) == value_counters->end()) {
      std::map<std::string, int> map;
      map[tokens[1]] = std::stoi(tokens[2]);
      value_counters->insert(
          std::pair<std::string, std::map<std::string, int>>(tokens[0], map));
    } else if (value_counters->find(tokens[0])->second.find(tokens[1]) ==
               value_counters->at(tokens[0]).end()) {
      value_counters->at(tokens[0]).insert(
          std::pair<std::string, int>(tokens[1], std::stoi(tokens[2])));
    } else {
      std::cerr << "Duplicate field/value pair in the file" << std::endl;
      throw std::exception();
    }
  }

  for (auto const &x : *value_counters) {
    std::cout << x.first << std::endl;
    for (auto const &y : x.second) {
      std::cout << " " << y.first << " " << y.second << std::endl;
    }
  }
}

Packet ClientProxy::forward(const Packet &inPacket) { return inPacket; }
