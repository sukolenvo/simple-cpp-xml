#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "simple_cpp_xml/parser.hpp"

class TestParser : public simple_cpp::xml::Parser
{
public:
  virtual ~TestParser() = default;
  std::vector<std::string> charData;
  std::vector<std::string> tagStart;
  std::vector<std::string> tagEnd;

protected:
  void on_start() override
  {}
  void on_tag_start(const std::vector<std::string> &tagXmlPath,
    const std::vector<simple_cpp::xml::Attribute> &attributes) override
  {
    std::string tagInfo = std::accumulate(tagXmlPath.begin(),
      tagXmlPath.end(),
      std::string("/"),
      [](const std::string &accumulator, const std::string &item) { return accumulator + item + std::string("/"); });
    for (const auto &[name, value] : attributes) {
      tagInfo += " ";
      tagInfo += name;
      tagInfo += "=";
      tagInfo += value;
    }
    tagStart.push_back(tagInfo);
  }
  void on_character_data([[maybe_unused]] const std::vector<std::string> &tagXmlPath, const std::string &data) override
  {
    charData.push_back(data);
  }
  void on_tag_end(const std::string &tagName) override
  {
    tagEnd.push_back(tagName);
  }
  void on_end() override
  {
    Parser::on_end();
  }
};

TEST_CASE("sample")
{
  // language=XML
  const std::string xml =
    R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?><!DOCTYPE report PUBLIC "-//JACOCO//DTD Report 1.1//EN" "report.dtd">
<report name="test-service">
  data
</report>
)";
  TestParser parser;
  parser.parse(xml);
  REQUIRE(parser.charData == std::vector{ std::string("data") });
  REQUIRE(parser.tagStart == std::vector{ std::string("/report/ name=test-service") });
}

auto readFile(auto path)
{
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();
    return buffer.str();
  } else {
    throw std::runtime_error("Failed to open task input file");
  }
}

auto readTestXml(const char *xml)
{
  std::array<char, 100> filename{};
  filename.fill(0);
  auto status = std::snprintf(filename.data(), filename.max_size(), "resources/%s.xml", xml);
  if (status < 0 || static_cast<size_t>(status) >= filename.max_size()) {
    throw std::runtime_error("Failed to construct task input filename");
  }
  return readFile(filename.data());
}

TEST_CASE("prolog")
{
  const std::string testXml = readTestXml("prolog");
  TestParser parser;
  parser.parse(testXml);
  REQUIRE(parser.charData == std::vector{ std::string("Hello, world!") });
  REQUIRE(parser.tagStart == std::vector{ std::string("/greeting/") });
}

TEST_CASE("emptyTag")
{
  const std::string testXml = readTestXml("emptyTag");
  TestParser parser;
  parser.parse(testXml);
  REQUIRE(parser.tagStart
          == std::vector{
            std::string("/test/"),
            std::string("/test/IMG/ align=left src=http://www.w3.org/Icons/WWW/w3c_home"),
            std::string("/test/br/"),
            std::string("/test/br/"),
          });
  REQUIRE(parser.charData.empty());
  REQUIRE(parser.tagEnd
          == std::vector{
            std::string("IMG"),
            std::string("br"),
            std::string("br"),
            std::string("test"),
          });
}

TEST_CASE("doctype")
{
  const char *testXml = GENERATE("doctypeNoNested", "doctypeWithNested");
  CAPTURE(testXml);
  TestParser parser;
  parser.parse(readTestXml(testXml));
  REQUIRE(parser.charData == std::vector{ std::string("Hello, world!") });
  REQUIRE(parser.tagStart == std::vector{ std::string("/greeting/") });
}

TEST_CASE("jacocoTestReport")
{
  const std::string testXml = readTestXml("jacocoTestReport");
  TestParser parser;
  parser.parse(testXml);
  REQUIRE(parser.charData.empty());
  REQUIRE(
    std::find(
      parser.tagStart.begin(), parser.tagStart.end(), "/report/counter/ type=INSTRUCTION missed=4026 covered=16506")
    != parser.tagStart.end());
}

TEST_CASE("coberturaTestReport")
{
  const std::string testXml = readTestXml("coberturaTestReport");
  TestParser parser;
  parser.parse(testXml);
  REQUIRE(parser.charData == std::vector<std::string>{ "../" });
  REQUIRE(
    std::find(parser.tagStart.begin(),
      parser.tagStart.end(),
      "/coverage/ line-rate=1.0 branch-rate=0.875 lines-covered=42 "
      "lines-valid=42 branches-covered=28 branches-valid=32 complexity=0.0 timestamp=1695511040 version=gcovr 5.0")
    != parser.tagStart.end());
}