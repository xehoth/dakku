target("dakku_gui")
  if is_plat("windows") then
    add_cxxflags("/Zc:__cplusplus")
  end
  set_kind("binary")
  add_rules("qt.widgetapp")
  add_headerfiles("*.h")
  add_files("*.cpp")
  add_files("mainwindow.ui")
  -- add files with Q_OBJECT meta (only for qt.moc)
  add_files("mainwindow.h")
  add_deps("dakku.core")