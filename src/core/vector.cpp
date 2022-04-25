#include <core/vector.h>

namespace dakku {

#define DAKKU_IMPLEMENT_VECTOR_TYPE(name, T)                                   \
  DAKKU_IMPLEMENT_LUA_OBJECT(name, []() {                                      \
    DAKKU_INFO("register " #name);                                             \
    auto &lua = Lua::instance().get_state();                                   \
    name (name::*negate)() const = &name::operator-;                           \
    name (name::*add_vector)(const name &) const = &name::operator+;           \
    name (name::*add_scalar)(T) const = &name::operator+<T>;                   \
    name (name::*sub_vector)(const name &) const = &name::operator-;           \
    name (name::*sub_scalar)(T) const = &name::operator-<T>;                   \
    name (name::*mul_vector)(const name &) const = &name::operator*;           \
    name (name::*mul_scalar)(T) const = &name::operator*<T>;                   \
    name (name::*div_vector)(const name &) const = &name::operator/;           \
    name (name::*div_scalar)(T) const = &name::operator/<T>;                   \
    lua.new_usertype<name>(                                                    \
        #name, sol::constructors<name(), name(T), name(const sol::table &)>(), \
        sol::meta_function::new_index, &name::set_by_index<T>, "clone",        \
        &name::clone, sol::meta_function::addition,                            \
        sol::overload(add_vector, add_scalar),                                 \
        sol::meta_function::unary_minus, negate,                               \
        sol::meta_function::multiplication,                                    \
        sol::overload(mul_vector, mul_scalar), sol::meta_function::division,   \
        sol::overload(div_vector, div_scalar),                                 \
        sol::meta_function::subtraction,                                       \
        sol::overload(sub_vector, sub_scalar));                                \
    return 0;                                                                  \
  })

DAKKU_IMPLEMENT_VECTOR_TYPE(Vector3f, float);
DAKKU_IMPLEMENT_VECTOR_TYPE(Vector3i, int);
DAKKU_IMPLEMENT_VECTOR_TYPE(Vector2f, float);
DAKKU_IMPLEMENT_VECTOR_TYPE(Vector2i, int);

#define DAKKU_IMPLEMENT_POINT_TYPE(name, T, vector_type)                       \
  DAKKU_IMPLEMENT_LUA_OBJECT(name, []() {                                      \
    DAKKU_INFO("register " #name);                                             \
    auto &lua = Lua::instance().get_state();                                   \
    name (name::*negate)() const = &name::operator-;                           \
    name (name::*add_vector)(const name &) const = &name::operator+;           \
    name (name::*add_scalar)(T) const = &name::operator+<T>;                   \
    name (name::*sub_vector)(const vector_type &) const = &name::operator-;    \
    vector_type (name::*sub_point)(const name &) const = &name::operator-;     \
    name (name::*sub_scalar)(T) const = &name::operator-<T>;                   \
    name (name::*mul_vector)(const name &) const = &name::operator*;           \
    name (name::*mul_scalar)(T) const = &name::operator*<T>;                   \
    name (name::*div_vector)(const name &) const = &name::operator/;           \
    name (name::*div_scalar)(T) const = &name::operator/<T>;                   \
    lua.new_usertype<name>(                                                    \
        #name, sol::constructors<name(), name(T), name(const sol::table &)>(), \
        sol::meta_function::new_index, &name::set_by_index<T>, "clone",        \
        &name::clone, sol::meta_function::addition,                            \
        sol::overload(add_vector, add_scalar),                                 \
        sol::meta_function::unary_minus, negate,                               \
        sol::meta_function::multiplication,                                    \
        sol::overload(mul_vector, mul_scalar), sol::meta_function::division,   \
        sol::overload(div_vector, div_scalar),                                 \
        sol::meta_function::subtraction,                                       \
        sol::overload(sub_vector, sub_scalar, sub_point));                     \
    return 0;                                                                  \
  })

DAKKU_IMPLEMENT_POINT_TYPE(Point3f, float, Vector3f);
DAKKU_IMPLEMENT_POINT_TYPE(Point3i, int, Vector3i);
DAKKU_IMPLEMENT_POINT_TYPE(Point2f, float, Vector2f);
DAKKU_IMPLEMENT_POINT_TYPE(Point2i, int, Vector2i);

DAKKU_IMPLEMENT_VECTOR_TYPE(Normal3f, float);
}  // namespace dakku