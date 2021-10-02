set(DAKKU_RENDER_SRC
        core/geometry.cpp
        core/transform.cpp
        core/spectrum.cpp
        core/film.cpp
        core/camera.cpp)

find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

add_library(dakku-render STATIC ${DAKKU_RENDER_SRC})
target_compile_features(dakku-render PRIVATE cxx_std_23)
target_link_libraries(
        dakku-render PUBLIC
        glm::glm
)
target_include_directories(dakku-render PRIVATE ${STB_INCLUDE_DIRS})
add_library(dakku::render ALIAS dakku-render)

add_subdirectory(main)
