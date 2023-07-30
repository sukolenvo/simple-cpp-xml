#include <stdexcept>

#include "string_reader.hpp"

char simple_cpp::xml::StringReader::read()
{
  check_eof();
  ++offset;
  return *current++;
}

char simple_cpp::xml::StringReader::peek() const
{
  check_eof();
  return *current;
}

std::string simple_cpp::xml::StringReader::peek(size_t count) const
{
  std::string result;
  result.resize(count);
  for (size_t i = 0; i < count; ++i) {
    if (current + i == end) {
      throw std::invalid_argument("EOF reached");
    }
    result[i] = *(current + i);
  }
  return result;
}

bool simple_cpp::xml::StringReader::eof() const
{
  return current == end;
}

std::string simple_cpp::xml::StringReader::position() const
{
  std::string result;
  if (current != end) {
    for (int i = 0; i < 7; ++i) {
      if (current + i != end) {
        result += *(current + i);
      } else {
        break;
      }
    }
    result += " ";
  }
  result += "at offset ";
  result += std::to_string(offset);
  return result + ": ";
}