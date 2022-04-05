add_requires("spdlog", {configs = {shared = true, debug = true}})
target("dakku.logger")
  add_includedirs(os.projectdir() .. "/src", {public = true})
  set_kind("shared")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_LOGGER_MODULE")
  add_files("*.cpp")
  add_packages("spdlog", {public = true})
