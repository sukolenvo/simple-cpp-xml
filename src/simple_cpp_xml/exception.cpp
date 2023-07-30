#include "exception.hpp"

simple_cpp::xml::ParseError::ParseError(const std::string &details) : message(details)
{}

const char *simple_cpp::xml::ParseError::what() const noexcept
{
  return message.c_str();
}