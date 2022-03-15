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
   * @return object to string (default: class name)
   */
  [[nodiscard]] virtual std::string toString() const;
  /**
   * @return the class name of the object
   */
  static constexpr std::string getClassNameStatic() { return "Object"; }
};

/**
 * declare a dakku object
 */
#define DAKKU_DECLARE_OBJECT(name)                                    \
  static constexpr std::string getClassNameStatic() { return #name; } \
  [[nodiscard]] std::string getClassName() const override {           \
    return getClassNameStatic();                                      \
  }                                                                   \
  explicit name() = default

DAKKU_END
#endif  // DAKKU_SRC_CORE_OBJECT_H_
