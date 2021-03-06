target("main")
  set_kind("binary")
  add_files("*.cpp")
  add_includedirs(os.projectdir() .. "/src", {public = true})
  add_defines("DAKKU_BUILD_MODULE=DAKKU_MAIN_MODULE")
  add_deps("dakku.filters")
  -- add_deps("dakku.core", "dakku.stream", "dakku.filters", "dakku.textures", "dakku.cameras")
  -- add_deps("dakku.math")