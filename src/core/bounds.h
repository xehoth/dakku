#ifndef DAKKU_SRC_CORE_BOUNDS_H_
#define DAKKU_SRC_CORE_BOUNDS_H_
#include <core/vector.h>
#include <iterator>

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

template <ArithmeticType T>
Bounds2<T> intersect(const Bounds2<T> &b1, const Bounds2<T> &b2) {
  // note: cannot direct use the constructor
  // because it will take min/max of the points
  Bounds2<T> ret;
  ret.pMin = max(b1.pMin, b2.pMin);
  ret.pMax = min(b1.pMax, b2.pMax);
  return ret;
}

template <ArithmeticType T>
bool insideExclusive(const Point2<T> &pt, const Bounds2<T> &b) {
  return (pt.x() >= b.pMin.x() && pt.x() < b.pMax.x() && pt.y() >= b.pMin.y() &&
          pt.y() < b.pMax.y());
}

class Bounds2iIterator : public std::forward_iterator_tag {
 public:
  explicit Bounds2iIterator(const Bounds2i &b, const Point2i &pt)
      : p(pt), bounds(&b) {}
  Bounds2iIterator operator++() {
    advance();
    return *this;
  }
  Bounds2iIterator operator++(int) {
    Bounds2iIterator old = *this;
    advance();
    return old;
  }
  bool operator==(const Bounds2iIterator &bi) const {
    return p == bi.p && bounds == bi.bounds;
  }
  bool operator!=(const Bounds2iIterator &bi) const {
    return p != bi.p || bounds != bi.bounds;
  }

  Point2i operator*() const { return p; }

 private:
  void advance() {
    ++p.x();
    if (p.x() == bounds->pMax.x()) {
      p.x() = bounds->pMin.x();
      ++p.y();
    }
  }
  Point2i p;
  const Bounds2i *bounds;
};

inline Bounds2iIterator begin(const Bounds2i &b) {
  return Bounds2iIterator(b, b.pMin);
}

inline Bounds2iIterator end(const Bounds2i &b) {
  // normally, the ending point is at the minimum x value and one past
  // the last valid y value.
  Point2i pEnd(b.pMin.x(), b.pMax.y());
  // however, if the bounds are degenerate, override the end point to
  // equal the start point so that any attempt to iterate over the bounds
  // exits out immediately.
  if (b.pMin.x() >= b.pMax.x() || b.pMin.y() >= b.pMax.y()) pEnd = b.pMin;
  return Bounds2iIterator(b, pEnd);
}

DAKKU_END
#endif  // DAKKU_SRC_CORE_BOUNDS_H_
