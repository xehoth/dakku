project(dakku-core LANGUAGES CXX)

find_package(Eigen3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB SRC_FILES "*.cpp" "*.h")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen spdlog::spdlog
        nlohmann_json::nlohmann_json)
add_library(dakku::core ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)
if (WIN32)
  set_target_properties(${PROJECT_NAME}
          PROPERTIES
          WINDOWS_EXPORT_ALL_SYMBOLS 1)
  target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif ()