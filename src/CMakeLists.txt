set(DAKKU_RENDER_SRC
        core/geometry.cpp
        core/transform.cpp
        core/spectrum.cpp
        core/film.cpp
        core/camera.cpp
        core/shape.cpp
        core/primitive.cpp
        core/light.cpp
        core/interaction.cpp
        core/scene.cpp
        core/parser.cpp
        core/integrator.cpp
        core/sampler.cpp
        core/material.cpp
        core/reflection.cpp
        core/sampling.cpp
        cameras/perspective.cpp
        shapes/triangle.cpp
        accelerators/embree_accel.cpp
        samplers/random.cpp
        materials/matte.cpp
        integrators/path.cpp
        lights/diffuse.cpp)

find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")
find_package(embree 3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(dakku-render STATIC ${DAKKU_RENDER_SRC})
target_compile_features(dakku-render PRIVATE cxx_std_23)
target_link_libraries(
        dakku-render PUBLIC
        glm::glm
        embree
)
target_link_libraries(dakku-render PRIVATE
        nlohmann_json
        nlohmann_json::nlohmann_json)
target_include_directories(dakku-render PRIVATE ${STB_INCLUDE_DIRS})
if (MSVC)
  target_compile_options(dakku-render PRIVATE "/openmp")
endif ()
add_library(dakku::render ALIAS dakku-render)

add_subdirectory(main)
