target("dakku.math")
  set_kind("headeronly")
  add_defines("DAKKU_BUILD_MODULE=DAKKU_MATH_MODULE")
  add_includedirs(os.projectdir() .. "/src", {interface = true})
