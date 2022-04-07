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
  }                                                            \
  explicit name() = default

/**
 * @brief export a object (register the class)
 *
 */
#define DAKKU_EXPORT_OBJECT(name)            \
  int _register##name = [] {                 \
    Class::instance().registerClass<name>(); \
    return 0;                                \
  }()

}  // namespace dakku
#endif