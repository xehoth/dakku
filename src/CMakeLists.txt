set(DAKKU_RENDER_SRC
  "core/geometry.cpp"
)

find_package(glm CONFIG REQUIRED)

add_library(dakku-render STATIC ${DAKKU_RENDER_SRC})
target_compile_features(dakku-render PRIVATE cxx_std_23)
target_link_libraries(
  dakku-render PUBLIC
  glm::glm
)
add_library(dakku::render ALIAS dakku-render)

add_subdirectory(main)
