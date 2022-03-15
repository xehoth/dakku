#ifndef DAKKU_SRC_CORE_BOUNDS_H_
#define DAKKU_SRC_CORE_BOUNDS_H_
#include <core/vector.h>

DAKKU_BEGIN
template <ArithmeticType T>
class Bounds2 {
 public:
  explicit Bounds2()
      : pMin(std::numeric_limits<T>::max()),
        pMax(std::numeric_limits<T>::lowest()) {}
  explicit Bounds2(const Point2<T> &p) : pMin(p), pMax(p) {}
  explicit Bounds2(const Point2<T> &p1, const Point2<T> &p2)
      : pMin(min(p1, p2)), pMax(max(p1, p2)) {}
  template <ArithmeticType U>
  explicit operator Bounds2<U>() const {
    return Bounds2<U>(Point2<U>(pMin), Point2<U>(pMax));
  }
  Vector2<T> diagonal() const { return pMax - pMin; }
  T area() const {
    auto d = diagonal();
    return d.x() * d.y();
  }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Bounds2, pMin, pMax);

  friend std::ostream &operator<<(std::ostream &os, const Bounds2 &value) {
    return os << "{pMin: " << value.pMin << ", pMax: " << value.pMax << "}";
  }

  Point2<T> pMin, pMax;
};

using Bounds2i = Bounds2<int>;
using Bounds2f = Bounds2<Float>;
DAKKU_END
#endif  // DAKKU_SRC_CORE_BOUNDS_H_
