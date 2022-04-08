add_requires("spdlog", {configs = {shared = true, debug = true}})
add_requires("nlohmann_json", {configs = {shared = true, debug = true}})

target("dakku.core")
  set_kind("shared")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_CORE_MODULE")
  add_includedirs(os.projectdir() .. "/src", {public = true})
  add_files("*.cpp")
  add_packages("spdlog", "nlohmann_json", {public = true})