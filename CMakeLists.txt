cmake_minimum_required(VERSION 3.22.1)

project(simple-cpp-xml VERSION 0.3)

cmake_policy(SET CMP0048 NEW)

option(OPT_ENABLE_COVERAGE "Enable code coverage")
option(SIMPLE_CPP_XML_WARNINGS_AS_ERRORS "Treat compiler warnings as errors")
add_library(simple-cpp-xml-compiler_options INTERFACE)
target_compile_features(simple-cpp-xml-compiler_options INTERFACE cxx_std_20)
if (MSVC)
  target_compile_options(simple-cpp-xml-compiler_options INTERFACE /W4)
  if (SIMPLE_CPP_XML_WARNINGS_AS_ERRORS)
    target_compile_options(simple-cpp-xml-compiler_options INTERFACE /WX)
  endif ()
else ()
  target_compile_options(simple-cpp-xml-compiler_options INTERFACE -Wall -Wextra -Wpedantic)
  if (SIMPLE_CPP_XML_WARNINGS_AS_ERRORS)
    target_compile_options(simple-cpp-xml-compiler_options INTERFACE -Werror)
  endif ()
  if (OPT_ENABLE_COVERAGE)
    if (CMAKE_COMPILER_IS_GNUCXX)
      target_compile_options(simple-cpp-xml-compiler_options INTERFACE -fprofile-arcs -ftest-coverage)
      target_link_options(simple-cpp-xml-compiler_options INTERFACE -lgcov --coverage)
    endif ()
  endif ()
endif ()

add_subdirectory(src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  enable_testing()
  include(FetchContent)
  FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0
      GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(Catch2)
  add_subdirectory(test)
  include(CTest)
endif ()