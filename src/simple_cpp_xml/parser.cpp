#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

#include "attribute.hpp"
#include "exception.hpp"
#include "parser.hpp"
#include "string_reader.hpp"

bool is_white_space(const char &c)
{
  return c == 0x20 || c == 0x9 || c == 0xD || c == 0xA;
}

bool is_name_start_char(int c)
{
  return c == ':' || (c >= 'A' && c <= 'Z') || c == '_' || (c >= 'a' && c <= 'z') || (c >= 0xC0 && c <= 0xD6)
         || (c >= 0xD8 && c <= 0xF6) || (c >= 0xF8 && c <= 0x2FF) || (c >= 0x30 && c <= 0x37D)
         || (c >= 0x27F && c < 0x1FFF) || (c >= 0xF900 && c <= 0xFDCF) || (c >= 0xFDF0 && c <= 0xFFFD)
         || (c >= 10000 && c <= 0xEFFFF);
}

bool is_name_char(int c)
{
  return is_name_start_char(c) || c == '-' || c == '.' || (c >= '0' && c <= '9') || c == 0xB7
         || (c >= 0x300 && c <= 0x36F) || (c >= 0x203F && c <= 0x2040);
}

void skip_white_spaces(simple_cpp::xml::Reader &reader)
{
  while (!reader.eof() && is_white_space(reader.peek())) {
    reader.read();
  }
}

struct StartTag
{
  bool empty{ false };
  std::string tag_name;
  std::vector<simple_cpp::xml::Attribute> attributes;
};

void verify_tag_name(const std::string &tagName)
{
  if (tagName.empty()) {
    throw std::invalid_argument("end tag name is empty");
  }
  if (!is_name_start_char(tagName[0])) {
    throw std::invalid_argument("tag name first char is invalid");
  }
  for (const auto c : tagName) {
    if (!is_name_char(c)) {
      throw std::invalid_argument("tag name includes invalid characters");
    }
  }
}

std::vector<simple_cpp::xml::Attribute> read_tag_attributes(simple_cpp::xml::Reader &reader)
{
  std::vector<simple_cpp::xml::Attribute> attributes;
  while (true) {
    skip_white_spaces(reader);
    if (reader.peek() == '>' || reader.peek() == '/') {
      break;
    }
    std::string name;
    name += reader.read();
    while (is_name_char(reader.peek()) && reader.peek() != '=') {
      name += reader.read();
    }
    std::string value;
    if (reader.peek() == '=') {
      reader.read();
      if (reader.read() != '"') {
        throw std::invalid_argument("attribute value is not enclosed in quotes");
      }
      while (true) {
        const auto next = reader.read();
        if (next == '"') {
          break;
        }
        value += next;
      }
    }
    attributes.emplace_back(name, value);
  }
  return attributes;
}

StartTag read_start_tag(simple_cpp::xml::Reader &reader)
{
  if (reader.read() != '<') {
    throw std::invalid_argument("expecting '<' to start tag");
  }
  StartTag startTag;
  while (true) {
    const auto next = reader.peek();
    if (is_white_space(next) || next == '/' || next == '>') {
      startTag.attributes = read_tag_attributes(reader);
      auto endChar = reader.read();
      if (endChar == '/') {
        startTag.empty = true;
        endChar = reader.read();
      }
      if (endChar != '>') {
        throw std::invalid_argument("expecting start tag to close with '>'");
      }
      verify_tag_name(startTag.tag_name);
      return startTag;
    }
    startTag.tag_name += reader.read();
  }
}

std::string read_end_tag(simple_cpp::xml::Reader &reader)
{
  if (reader.read() != '<' || reader.read() != '/') {
    throw std::invalid_argument("expecting '</' to end tag");
  }
  std::string result;
  while (true) {
    const auto next = reader.read();
    if (next == '>') {
      verify_tag_name(result);
      return result;
    }
    result += next;
  }
}

bool iequals(const std::string &first, const std::string &second)
{
  if (first.size() != second.size()) {
    return false;
  }
  for (size_t i = 0; i < first.size(); ++i) {
    if (std::tolower(first[i]) != std::tolower(second[i])) {
      return false;
    }
  }
  return true;
}

void read_xml_declaration_if_any(simple_cpp::xml::Reader &reader)
{
  skip_white_spaces(reader);
  if (iequals(reader.peek(5), "<?xml")) {
    while (reader.read() != '>') {
      // skip till the end of the tag
    }
  }
}

void read_doctype_if_any(simple_cpp::xml::Reader &reader)
{
  skip_white_spaces(reader);
  if (reader.peek(9) == std::string("<!DOCTYPE")) {
    std::string endMarker{ ">" };
    size_t positionInMarker{ 0 };
    while (true) {
      auto next = reader.read();
      if (next == '[') {
        endMarker = "]>";
        positionInMarker = 0;
      } else if (next == endMarker[positionInMarker]) {
        ++positionInMarker;
        if (positionInMarker == endMarker.size()) {
          break;
        }
      }
    }
  }
}

std::string read_character_data(simple_cpp::xml::Reader &reader)
{
  // TODO add support &amp; &lt; &gt; &apos; &quot;
  skip_white_spaces(reader);
  std::string value;
  while (reader.peek() != '<') {
    value += reader.read();
  }
  value.erase(std::find_if_not(value.rbegin(), value.rend(), is_white_space).base(), value.end());
  return value;
}

void simple_cpp::xml::Parser::parse(const std::string_view &xml)
{
  simple_cpp::xml::StringReader reader(xml);
  try {
    on_start();
    read_xml_declaration_if_any(reader);
    read_doctype_if_any(reader);
    skip_white_spaces(reader);
    std::vector<std::string> xmlPath;
    if (reader.peek() != '<') {
      throw std::invalid_argument("expecting to start tag");
    }
    while (!reader.eof()) {
      skip_white_spaces(reader);
      const auto next = reader.peek();
      if (next == '<') {
        if (reader.peek(2)[1] == '/') {
          const auto endTag = read_end_tag(reader);
          if (xmlPath.empty() || xmlPath.back() != endTag) {
            throw std::invalid_argument("xml mailformed: end tag don't match start tag");
          }
          on_tag_end(endTag);
          xmlPath.pop_back();
        } else {
          const auto startTag = read_start_tag(reader);
          xmlPath.push_back(startTag.tag_name);
          on_tag_start(xmlPath, startTag.attributes);
          if (startTag.empty) {
            on_tag_end(startTag.tag_name);
            xmlPath.pop_back();
          }
        }
      } else {
        const auto data = read_character_data(reader);
        on_character_data(xmlPath, data);
      }
      skip_white_spaces(reader);
    }
    on_end();
  } catch (const std::exception &e) {
    throw simple_cpp::xml::ParseError(reader.position() + std::string(e.what()));
  }
}
