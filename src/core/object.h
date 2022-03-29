#ifndef DAKKU_SRC_CORE_OBJECT_H_
#define DAKKU_SRC_CORE_OBJECT_H_
#include <core/fwd.h>
#include <string>

DAKKU_BEGIN
/**
 * dakku Object
 */
class Object {
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
  [[nodiscard]] bool isDerivedFrom(std::string_view name) const;
  [[nodiscard]] bool isBaseOf(std::string_view name) const;
  /**
   * @return the class name of the object
   */
  static constexpr std::string getClassNameStatic() { return "Object"; }
  /**
   * @return the class name of the parent of the object
   */
  static constexpr std::string getParentNameStatic() { return ""; }
};

/**
 * declare a dakku object
 */
#define DAKKU_DECLARE_OBJECT(name, parent)                               \
  static constexpr std::string getClassNameStatic() { return #name; }    \
  [[nodiscard]] std::string getClassName() const override {              \
    return getClassNameStatic();                                         \
  }                                                                      \
  static constexpr std::string getParentNameStatic() { return #parent; } \
  [[nodiscard]] std::string getParentName() const override {             \
    return getParentNameStatic();                                        \
  }                                                                      \
  explicit name() = default

DAKKU_END
#endif  // DAKKU_SRC_CORE_OBJECT_H_
