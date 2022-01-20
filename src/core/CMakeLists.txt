project(dakku-core LANGUAGES CXX)

CPMAddPackage("gh:mitsuba-renderer/enoki#2a18afa2402e0677887c8439fa3d6a270ea15726")
if (enoki_ADDED)
  add_library(enoki INTERFACE IMPORTED)
  target_include_directories(enoki INTERFACE ${enoki_SOURCE_DIR}/include)
endif()

file(GLOB SRC_FILES "*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(
  ${PROJECT_NAME} PUBLIC
  dakku::logger
  enoki
)
add_library(dakku::core ALIAS ${PROJECT_NAME})