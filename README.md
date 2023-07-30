# Simple C++ XML parser

C++ library for parsing XML.

Following features of XML are **not** supported:
* prolog ignored
* DTD (!DOCTYPE tag ignored)
* UTF-8 encoding
* comments
* CDATA
* [Processing Instructions](https://en.wikipedia.org/wiki/Processing_Instruction) 

## Usage

```cpp
#include <iostream>

#include <simple_cpp_xml/parser.hpp>

class BookParser : public simple_cpp::xml::Parser
{

protected:
  void on_tag_start(const std::vector<std::string> &tagXmlPath,
    const std::vector<simple_cpp::xml::Attribute> &attributes) override
  {
    std::cout << "on_tag_start: /";
    for (const auto &tag : tagXmlPath) {
      std::cout << tag << '/';
    }
    for (const auto &[name, value] : attributes) {
      std::cout << ' ' << name << '=' << value;
    }
    std::cout << '\n';
  }
  void on_character_data([[maybe_unused]] const std::vector<std::string> &tagXmlPath, const std::string &data) override
  {
    std::cout << "on_character_data: " << data << '\n';
  }
};

BookParser parser;
parser.parse(xml);
```

For a sample XML:

```xml
<?xml version="1.0"?>
<book location="https://example.com/Lord-Rings-J-R-Tolkien">
  <title>The Lord of the Rings</title>
  <author>John Ronald Reuel Tolkien</author>
</book>
```

Output would be:
```
on_tag_start: /book/ location=https://example.com/Lord-Rings-J-R-Tolkien
on_tag_start: /book/title/
on_character_data: The Lord of the Rings
on_tag_start: /book/author/
on_character_data: John Ronald Reuel Tolkien
```

## Cmake integration

### Git submodule
Checkout project as a Git submodule, then update `CMakeLists.txt`:
```cmake
add_subdirectory(vendor/simple-cpp-xml)

target_link_libraries(target simple-cpp-xml::lib) 
```

### FetchContent
```cmake
include(FetchContent)

FetchContent_Declare(
    simple-cpp-base64
    GIT_REPOSITORY https://github.com/sukolenvo/simple-cpp-xml.git
    GIT_TAG        v0.1
)

FetchContent_MakeAvailable(simple-cpp-xml)

target_link_libraries(target simple-cpp-xml::lib) 
```
