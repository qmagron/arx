#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <exception>
#include <string>
#include <string.h>


class Exception: public std::exception {
 private:
  const std::string message;

 public:
  Exception(const std::string& message) noexcept;
  ~Exception() noexcept = default;

  const char* what() const noexcept;
};

class CException: public Exception {
 public:
  CException(int nErr) noexcept;
  ~CException() noexcept = default;
};


inline Exception::Exception(const std::string& message) noexcept: message(message) {}
inline CException::CException(int nErr) noexcept: Exception(strerror(nErr)) {}

inline const char* Exception::what() const noexcept {
  return this->message.c_str();
}


#endif
