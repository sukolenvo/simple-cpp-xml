#ifndef SIMPLE_CPP_XML_PARSER_HPP
#define SIMPLE_CPP_XML_PARSER_HPP

#include <string>
#include <string_view>
#include <vector>

#include "attribute.hpp"

namespace simple_cpp::xml {

// https://www.w3.org/TR/xml/
class Parser
{
public:
  virtual ~Parser() = default;
protected:
  virtual void on_start()
  {}
  virtual void on_tag_start([[maybe_unused]] const std::vector<std::string> &tagXmlPath,
    [[maybe_unused]] const std::vector<simple_cpp::xml::Attribute> &attributes)
  {}
  virtual void on_character_data([[maybe_unused]] const std::vector<std::string> &tagXmlPath,
    [[maybe_unused]] const std::string &data)
  {}
  virtual void on_tag_end([[maybe_unused]] const std::string &tagName)
  {}
  virtual void on_end()
  {}
public:
  void parse(const std::string_view &xml);
};

} // namespace simple_cpp::xml

#endif // SIMPLE_CPP_XML_PARSER_HPP
