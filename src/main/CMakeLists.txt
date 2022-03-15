project(dakku LANGUAGES CXX)

find_package(cxxopts CONFIG REQUIRED)

file(GLOB SRC_FILES "*.cpp" "*.h")
add_executable(dakku ${SRC_FILES})
target_include_directories(dakku PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(dakku PRIVATE cxx_std_20)
target_link_libraries(dakku PRIVATE cxxopts::cxxopts dakku::core)
set_target_properties(dakku PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)
if (WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

  set_target_properties(dakku
          PROPERTIES
          WINDOWS_EXPORT_ALL_SYMBOLS 1)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif ()
