#ifndef DAKKU_CORE_BOUNDS_H_
#define DAKKU_CORE_BOUNDS_H_
#include <core/vector.h>

#include <iterator>
#include <utility>

namespace dakku {

/**
 * @brief bounding box base
 *
 * @tparam T data type
 * @tparam S dimension
 */
template <ArithmeticType T, size_t S>
class BoundsBase {
 public:
  /**
   * @brief default constructor
   *
   */
  BoundsBase() = default;

  /// left bottom corner of the bounds
  Point<T, S> pMin{std::numeric_limits<T>::max()};
  /// right top corner of the bounds
  Point<T, S> pMax{std::numeric_limits<T>::lowest()};
};
}  // namespace dakku
#endif