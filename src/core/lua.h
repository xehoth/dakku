#ifndef DAKKU_CORE_LUA_H_
#define DAKKU_CORE_LUA_H_
#include <core/fwd.h>

#if !defined(SOL_ALL_SAFETIES_ON)
#define SOL_ALL_SAFETIES_ON 1
#endif
#include <sol/sol.hpp>

namespace dakku {

/**
 * @brief dakku lua state
 *
 */
class DAKKU_EXPORT_CORE Lua {
 public:
  /**
   * @brief get lua instance
   *
   */
  static Lua &instance();

  /**
   * @brief get lua state
   *
   */
  sol::state &get_state();

 private:
  sol::state state;
};

#define DAKKU_DECLARE_LUA_OBJECT(name) \
  struct _##name##_lua_bind_wrapper {  \
    static int _##name##_bind_lua;     \
  }

#define DAKKU_IMPLEMENT_LUA_OBJECT(name, func) \
  int _##name##_lua_bind_wrapper::_##name##_bind_lua = (func)()

}  // namespace dakku
#endif