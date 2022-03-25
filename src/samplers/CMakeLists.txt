project(dakku-samplers LANGUAGES CXX)

find_package(libigl CONFIG REQUIRED)

file(GLOB SRC_FILES "*.cpp" "*.h")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC dakku::core)
target_link_libraries(${PROJECT_NAME} PRIVATE igl::common igl::core)
add_library(dakku::samplers ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)
if (WIN32)
  set_target_properties(${PROJECT_NAME}
          PROPERTIES
          WINDOWS_EXPORT_ALL_SYMBOLS 1)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif ()