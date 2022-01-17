project(dakku-gui LANGUAGES CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

file(GLOB SRC_FILES "*.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE 
  glfw GLEW::GLEW
  dakku::logger
)
add_library(dakku::gui ALIAS ${PROJECT_NAME})