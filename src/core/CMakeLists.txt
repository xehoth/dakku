project(dakku-core LANGUAGES CXX)

find_package(Eigen3 CONFIG REQUIRED)

file(GLOB SRC_FILES "*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(
  ${PROJECT_NAME} PUBLIC
  dakku::logger
  Eigen3::Eigen
)
add_library(dakku::core ALIAS ${PROJECT_NAME})