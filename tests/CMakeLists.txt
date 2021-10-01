project(dakku-tests LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

file(GLOB_RECURSE DAKKU_TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${DAKKU_TEST_SRC})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  dakku::render
  GTest::gtest
  GTest::gtest_main
)