#include <core/logger.h>
#include <core/lua.h>

#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  Lua::instance().get_state().open_libraries(sol::lib::base);
  Lua::instance().get_state().script_file("../../../../scenes/test.lua");
  return 0;
}