set_project("dakku")
set_languages("c++20")
add_rules("mode.debug", "mode.release")
if is_mode("debug") then 
  add_defines("DAKKU_BUILD_DEBUG", "DAKKU_BUILD_CHECK")
end
add_vectorexts("mmx", "sse", "sse2", "sse3", "ssse3", "avx", "avx2")
includes("src")