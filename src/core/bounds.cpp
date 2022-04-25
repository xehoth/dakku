#include <core/bounds.h>

namespace dakku {

DAKKU_IMPLEMENT_LUA_OBJECT(Bounds, [] {
  DAKKU_INFO("register Bounds");
  auto &lua = Lua::instance().get_state();
  lua.new_usertype<Bounds2i>(
      "Bounds2i", sol::constructors<Bounds2i(), Bounds2i(const sol::table &)>(),
      "p_min", &Bounds2i::p_min, "p_max", &Bounds2i::p_max);
  lua.new_usertype<Bounds2f>(
      "Bounds2f", sol::constructors<Bounds2f(), Bounds2f(const sol::table &)>(),
      "p_min", &Bounds2f::p_min, "p_max", &Bounds2f::p_max);
  lua.new_usertype<Bounds3f>(
      "Bounds3f", sol::constructors<Bounds3f(), Bounds3f(const sol::table &)>(),
      "p_min", &Bounds3f::p_min, "p_max", &Bounds3f::p_max);
  return 0;
});
}