#ifndef DAKKU_SRC_CORE_CLASS_H_
#define DAKKU_SRC_CORE_CLASS_H_
#include <core/fwd.h>
#include <core/singleton.h>
#include <core/logger.h>
#include <map>
#include <string_view>

DAKKU_BEGIN
/**
 * dakku RTTI
 */
class DAKKU_EXPORT_CORE Class final : public Singleton<Class> {
 public:
  /**
   * register an object class
   * @tparam T object type
   */
  template <ObjectType T>
  void registerClass();

  /**
   * create an object with given name
   * @param name the name of the object
   * @return the object instance
   */
  Object *create(std::string_view name);

 private:
  friend class Singleton<Class>;
  explicit Class() = default;

  template <typename T>
  static Object *constructor();

  std::map<std::string, std::add_pointer_t<Object *()>, std::less<>> _classMap;
};

template <ObjectType T>
void Class::registerClass() {
  if (!_classMap.contains(T::getClassNameStatic())) {
    DAKKU_INFO("register class: {}", T::getClassNameStatic());
    _classMap[T::getClassNameStatic()] = &constructor<T>;
  } else {
    DAKKU_WARN("class {} has already been registered", T::getClassNameStatic());
  }
}

template <typename T>
Object *Class::constructor() {
  return new T;
}

/**
 * macro for register a class
 */
#define DAKKU_REGISTER_CLASS(name) Class::instance().registerClass<name>()

DAKKU_END
#endif  // DAKKU_SRC_CORE_CLASS_H_