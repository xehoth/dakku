set(DAKKU_MODULE_CORE 1)
set(DAKKU_MODULE_FILTERS 2)
set(DAKKU_MODULE_GUI 3)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/defines.h.in ${CMAKE_CURRENT_SOURCE_DIR}/defines.h)

add_subdirectory(core)
add_subdirectory(filters)
add_subdirectory(shapes)
add_subdirectory(accelerators)
add_subdirectory(textures)
add_subdirectory(materials)
add_subdirectory(lights)
add_subdirectory(main)