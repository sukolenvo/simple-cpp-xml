#ifndef SIMPLE_CPP_XML_ATTRIBUTE_HPP
#define SIMPLE_CPP_XML_ATTRIBUTE_HPP

#include <string>

namespace simple_cpp::xml {

struct Attribute
{
  std::string name;
  std::string value;

  Attribute(const auto &name_, const auto &value_) : name(name_), value(value_) {}
};
} // namespace simple_cpp::xml

#endif // SIMPLE_CPP_XML_ATTRIBUTE_HPP
