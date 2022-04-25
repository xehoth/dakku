#include <core/logger.h>
#include <core/vector.h>
#include <core/lua.h>

#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  Lua::instance().get_state().open_libraries(sol::lib::base);
  Vector3f test =
      Lua::instance().get_state().script_file("../../../../scenes/test.lua");
  std::cout << test;
  return 0;
}