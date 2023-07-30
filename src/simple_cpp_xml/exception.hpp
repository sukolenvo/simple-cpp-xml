#ifndef SIMPLE_CPP_XML_EXCEPTION_HPP
#define SIMPLE_CPP_XML_EXCEPTION_HPP

#include <exception>
#include <string>

namespace simple_cpp::xml {
class ParseError : public std::exception
{
  std::string message;

public:
  explicit ParseError(const std::string &details);
  const char *what() const noexcept override;
};
} // namespace simple_cpp::xml

#endif // SIMPLE_CPP_XML_EXCEPTION_HPP