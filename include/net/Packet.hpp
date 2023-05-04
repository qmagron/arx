#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <istream>
#include <cstdlib>
#include <cstring>


struct Packet {
  size_t size;
  void* data = nullptr;

  Packet() = default;

  inline Packet(const std::string& str) {
    this->size = str.size();
    this->data = malloc(this->size);
    memcpy(this->data, str.c_str(), this->size);
  }

  inline Packet(size_t integer) {
    this->size = sizeof(size_t);
    this->data = malloc(this->size);
    memcpy(this->data, &integer, this->size);
  }

  inline ~Packet() {
    free(this->data);
  }

  inline Packet(const Packet& other) noexcept {
    this->size = other.size;
    this->data = malloc(other.size);
    memcpy(this->data, other.data, other.size);
  }

  inline Packet(Packet&& other) noexcept {
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
  }
};


#endif
