#include <core/lua.h>

namespace dakku {

Lua &Lua::instance() {
  static Lua _instance;
  return _instance;
}

sol::state &Lua::get_state() { return this->state; }
}  // namespace dakku