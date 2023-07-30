#ifndef SIMPLE_CPP_XML_READER_HPP
#define SIMPLE_CPP_XML_READER_HPP

#include <string>

namespace simple_cpp::xml {

class Reader
{
public:
  [[nodiscard]] virtual std::string position() const = 0;
  virtual char read() = 0;
  [[nodiscard]] virtual char peek() const = 0;
  [[nodiscard]] virtual std::string peek(size_t count) const = 0;
  [[nodiscard]] virtual bool eof() const = 0;
};

} // namespace simple_cpp::xml

#endif // SIMPLE_CPP_XML_READER_HPP
