cmake_minimum_required(VERSION 3.21)
project(dakku LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(CPM)

if (USE_SANITIZER OR USE_STATIC_ANALYZER)
  CPMAddPackage("gh:StableCoder/cmake-scripts#1f822d1fc87c8d7720c074cde8a278b44963c354")

  if (USE_SANITIZER)
    include(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)
  endif()
endif()

add_subdirectory(src)
add_subdirectory(tests)