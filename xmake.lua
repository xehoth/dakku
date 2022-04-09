set_project("dakku")
set_languages("c++20")
set_warnings("allextra")
add_rules("mode.debug", "mode.release", "mode.check")
set_policy("package.requires_lock", true)

if is_mode("debug") then 
  add_defines("DAKKU_BUILD_DEBUG", "DAKKU_BUILD_CHECK")
end

add_vectorexts("mmx", "sse", "sse2", "sse3", "ssse3", "avx", "avx2")
includes("src")

option("build_test")
  set_default(true)
  set_showmenu(true)
  set_description("build with tests or not")

if has_config("build_test") then
  includes("tests")
end