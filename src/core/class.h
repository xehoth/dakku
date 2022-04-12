#ifndef DAKKU_CORE_CLASS_H_
#define DAKKU_CORE_CLASS_H_
#include <core/fwd.h>
#include <core/logger.h>
#include <core/property.h>
#include <string>
#include <map>
#include <string_view>

namespace dakku {

/**
 * @brief RTTI support
 *
 */
class DAKKU_EXPORT_CORE Class final {
 public:
  /**
   * @brief get the class instance (singleton)
   *
   * @return the instance
   */
  static Class &instance();

  /**
   * @brief register an object class
   *
   * @tparam T object type
   */
  template <ObjectType T>
  void registerClass();

  /**
   * @brief check whether `cur` dervies from `from`
   *
   */
  [[nodiscard]] bool isDerivedFrom(std::string_view cur,
                                   std::string_view from) const;

  /**
   * @brief check whether `cur` is base of `other`
   *
   */
  [[nodiscard]] bool isBaseof(std::string_view cur,
                              std::string_view other) const;

  /**
   * @brief check whether `name` is registered
   *
   */
  [[nodiscard]] bool contains(std::string_view name) const;

  /**
   * @brief create an object instance and construct with property
   *
   * @param name object name
   * @return the object
   */
  Object *create(std::string_view name, const Property & = {});

 private:
  explicit Class() = default;

  /**
   * @brief constructor helper
   *
   */
  template <typename T>
  static Object *constructor(const Property &);

  struct Metadata {
    std::add_pointer_t<Object *(const Property &)> constructor;
    std::string parent;
  };

  std::map<std::string, Metadata, std::less<>> _classMap;
};

template <ObjectType T>
void Class::registerClass() {
  if (!_classMap.contains(T::getClassNameStatic())) {
    DAKKU_INFO("register class({}): {}, parent: {}",
               (std::is_constructible_v<T, const Property &> ? "c" : "u"),
               T::getClassNameStatic(), T::getParentNameStatic());
    _classMap[T::getClassNameStatic()] = {&constructor<T>,
                                          T::getParentNameStatic()};
  } else {
    DAKKU_WARN("class {} has already been registered", T::getClassNameStatic());
  }
}

template <typename T>
Object *Class::constructor(const Property &property) {
  if constexpr (std::is_constructible_v<T, const Property &>) {
    return new T{property};
  } else {
    DAKKU_ERR("cannot construct {}", T::getClassNameStatic());
    return nullptr;
  }
}
}  // namespace dakku
#endif