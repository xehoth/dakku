add_requires("gtest", {configs = {main = true}})
target("test")
  set_kind("binary")
  add_files("*.cpp")
  add_packages("gtest")
  add_deps("dakku.core", "dakku.stream")