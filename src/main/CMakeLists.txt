add_executable(dakku dakku.cpp)
target_compile_features(dakku PRIVATE cxx_std_23)
target_link_libraries(dakku PRIVATE dakku::render)
