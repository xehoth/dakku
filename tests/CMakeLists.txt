project(dakku-tests LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  dakku::core
  GTest::gtest GTest::gtest_main
)
