#ifndef DAKKU_CORE_CLASS_H_
#define DAKKU_CORE_CLASS_H_
#include <core/fwd.h>

namespace dakku {

/**
 * @brief RTTI support
 *
 */
class DAKKU_EXPORT_CORE Class {
 public:
  /**
   * @brief get the class instance (singleton)
   *
   * @return the instance
   */
  static Class &instance();

 private:
};
}  // namespace dakku
#endif