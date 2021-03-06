add_requires("spdlog")
add_requires("sol2")
add_requires("onetbb")

target("dakku.core")
  set_kind("shared")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_CORE_MODULE")
  add_includedirs(os.projectdir() .. "/src", {public = true})
  add_files("*.cpp")
  add_packages("spdlog", "sol2", "onetbb", {public = true})