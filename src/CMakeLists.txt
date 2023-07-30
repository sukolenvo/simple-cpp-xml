add_library(simple-cpp-xml-lib simple_cpp_xml/parser.cpp simple_cpp_xml/string_reader.cpp simple_cpp_xml/exception.cpp)
target_link_libraries(simple-cpp-xml-lib PRIVATE simple-cpp-xml-compiler_options)
target_include_directories(simple-cpp-xml-lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(simple-cpp-xml::lib ALIAS simple-cpp-xml-lib)