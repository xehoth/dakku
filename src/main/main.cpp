#include <core/logger.h>
#include <core/lua.h>

#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
#if defined(_WIN32) || defined(WIN32)
  LoadLibrary("dakku.core.dll");
  LoadLibrary("dakku.filters.dll");
#endif
  Lua::instance().get_state().open_libraries(sol::lib::base);
  Lua::instance().get_state().script_file("../../../../scenes/test.lua");
  return 0;
}