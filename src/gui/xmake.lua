target("dakku_gui")
  if is_plat("windows") then
    add_cxxflags("/Zc:__cplusplus")
    add_ldflags("/SUBSYSTEM:CONSOLE")
  end
  add_defines("QT_NO_KEYWORDS")
  set_kind("binary")
  add_rules("qt.quickapp")
  add_files("*.cpp")
  add_files("engine.h", "film.h")
  add_files("qml.qrc")
  add_deps("dakku.core")