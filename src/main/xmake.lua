target("main")
  set_kind("binary")
  add_files("*.cpp")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_MAIN_MODULE")
  add_deps("dakku.core")