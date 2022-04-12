#ifndef DAKKU_CORE_OBJECT_H_
#define DAKKU_CORE_OBJECT_H_
#include <core/class.h>

namespace dakku {

/**
 * @brief Object class, RTTI support
 *
 */
class DAKKU_EXPORT_CORE Object {
 public:
  virtual ~Object() = default;

  explicit Object(const Property & = {});

  /**
   * @return the class name of the object
   */
  [[nodiscard]] virtual std::string getClassName() const;

  /**
   * @return the class name of the parent of the object
   */
  [[nodiscard]] virtual std::string getParentName() const;

  /**
   * @return object to string (default: class name)
   */
  [[nodiscard]] virtual std::string toString() const;

  /**
   * @brief whther the object is derived from `name` (inclusive)
   *
   */
  [[nodiscard]] bool isDerivedFrom(std::string_view name) const;

  /**
   * @brief whether the object is base of `name` (inclusive)
   *
   */
  [[nodiscard]] bool isBaseOf(std::string_view name) const;

  /**
   * @return the class name of the object
   */
  static std::string getClassNameStatic() { return "Object"; }
  /**
   * @return the class name of the parent of the object
   */
  static std::string getParentNameStatic() { return ""; }
};

/**
 * @brief declare a dakku object
 */
#define DAKKU_DECLARE_OBJECT(name, parent)                     \
  static std::string getClassNameStatic() { return #name; }    \
  [[nodiscard]] std::string getClassName() const override {    \
    return getClassNameStatic();                               \
  }                                                            \
  static std::string getParentNameStatic() { return #parent; } \
  [[nodiscard]] std::string getParentName() const override {   \
    return getParentNameStatic();                              \
  }

template <typename... Args>
inline void registerObjects() {
  (Class::instance().registerClass<Args>(), ...);
}

#define DAKKU_EXPORT_OBJECTS(module, ...)            \
  volatile int _export##module##ModuleObjects = [] { \
    registerObjects<__VA_ARGS__>();                  \
    return 0;                                        \
  }()

}  // namespace dakku
#endif