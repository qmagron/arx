#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <string>
#include <exception>


class Exception: public std::exception {
 private:
  constexpr static int OK = 0;

  const std::string message;
  const int nErr;

 public:
  Exception(const std::string& message, int nErr = OK) noexcept;
  ~Exception() noexcept = default;

  const char* what() const noexcept;
};


#endif
