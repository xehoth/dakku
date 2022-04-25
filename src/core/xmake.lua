add_requires("spdlog")
add_requires("sol2")

target("dakku.core")
  set_kind("shared")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_CORE_MODULE")
  add_includedirs(os.projectdir() .. "/src", {public = true})
  add_files("*.cpp")
  add_packages("spdlog", "sol2", {public = true})