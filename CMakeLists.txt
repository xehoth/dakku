cmake_minimum_required(VERSION 3.21)
project(dakku DESCRIPTION "dakku renderer" LANGUAGES CXX)

option(DAKKU_BUILD_TESTS "build tests" on)

include_directories(include)

add_subdirectory(src)

if (DAKKU_BUILD_TESTS)
  add_subdirectory(tests)
endif()
