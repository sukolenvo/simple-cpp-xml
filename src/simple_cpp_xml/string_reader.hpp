#ifndef SIMPLE_CPP_XML_STRING_READER_HPP
#define SIMPLE_CPP_XML_STRING_READER_HPP

#include <stdexcept>
#include <string_view>

#include "reader.hpp"

namespace simple_cpp::xml {

class StringReader : public simple_cpp::xml::Reader
{
  std::string_view::const_iterator current;
  std::string_view::const_iterator end;
  size_t offset{ 0 };

  void check_eof() const
  {
    if (current == end) {
      throw std::invalid_argument("EOF reached");
    }
  }

public:
  StringReader(const std::string_view &data) : current(data.begin()), end(data.end())
  {}

  std::string position() const override;
  char read() override;
  char peek() const override;
  std::string peek(size_t count) const override;
  bool eof() const override;
};

} // namespace simple_cpp::xml

#endif // SIMPLE_CPP_XML_STRING_READER_HPP
