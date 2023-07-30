add_executable(simple-cpp-xml-tests test.cpp)
target_link_libraries(simple-cpp-xml-tests PRIVATE Catch2::Catch2WithMain simple-cpp-xml-compiler_options simple-cpp-xml-lib)

include(Catch)
catch_discover_tests(simple-cpp-xml-tests)
add_custom_command(TARGET simple-cpp-xml-tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources)