#include "Exception.hpp"


Exception::Exception(const std::string& message, int nErr) noexcept: message(message), nErr(nErr) {}


const char* Exception::what() const noexcept {
  if (this->nErr != OK) {
    return (this->message + ": " + std::to_string(this->nErr)).c_str();
  } else {
    return this->message.c_str();
  }
}
