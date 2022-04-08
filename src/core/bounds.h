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

  /**
   * @brief Construct a new Bounds2 with a point
   *
   * @param p the given point
   */
  explicit BoundsBase(const Point<T, S> &p) : pMin(p), pMax(p) {}

  /**
   * @brief type conversion
   *
   */
  template <ArithmeticType U>
  explicit BoundsBase(const BoundsBase<U, S> &b)
      : pMin(Point<T, S>(b.pMin)), pMax(Point<T, S>(b.pMax)) {}

  /**
   * @brief Construct a new Bounds Base object with two points
   * (take min/max of them to pMin and pMax)
   *
   */
  BoundsBase(const Point<T, S> &p1, const Point<T, S> &p2)
      : pMin(min(p1, p2)), pMax(max(p1, p2)) {}

  /**
   * @brief get the diagonal of the bounding box
   *
   */
  [[nodiscard]] Vector<T, S> diagonal() const { return pMax - pMin; }

  /**
   * @brief get the dimension who has the maximum extent
   *
   * @return the dimension
   */
  [[nodiscard]] size_t maxExtent() const {
    return this->diagonal().maxElementIndex();
  }

  bool operator==(const BoundsBase &rhs) const {
    return pMin == rhs.pMin && pMax == rhs.pMax;
  }

  bool operator!=(const BoundsBase &rhs) const {
    return pMin != rhs.pMin || pMax != rhs.pMax;
  }

  /**
   * @brief linera interpolation between pMin and pMax
   *
   * @param t interpolate $t$ (element wise)
   */
  [[nodiscard]] Point<T, S> lerp(const Point<float, S> &t) const {
    Point<T, S> ret;
    for (size_t i = 0; i < S; ++i) ret[i] = std::lerp(pMin[i], pMax[i], t[i]);
    return ret;
  }

  /**
   * @brief get the offset $v$ of a point, p = o + v * d
   *
   * @param p the point
   * @return the offset
   */
  [[nodiscard]] Vector<T, S> offset(const Point<T, S> &p) const {
    Vector<T, S> o = p - pMin;
    for (size_t i = 0; i < S; ++i)
      if (pMax[i] > pMin[i]) o[i] /= pMax[i] - pMin[i];
    return o;
  }

  /**
   * @brief check whether two bounds overlaps
   *
   */
  friend bool overlaps(const BoundsBase &a, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(a.pMax[i] >= b.pMin[i] && a.pMin[i] <= b.pMax[i])) return false;
    return true;
  }

  /**
   * @brief check whether point is inside the bounds (exclusive)
   *
   * @param p the point
   * @param b the bounds
   */
  friend bool inside(const Point<T, S> &p, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(p[i] >= b.pMin[i] && p[i] <= b.pMax[i])) return false;
    return true;
  }

  /**
   * @brief check whether point is inside the bounds (inclusive)
   *
   * @param p the point
   * @param b the bounds
   */
  friend bool insideExclusive(const Point<T, S> &p, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(p[i] >= b.pMin[i] && p[i] < b.pMax[i])) return false;
    return true;
  }

  /**
   * @brief minimum squared distance between point and bounds, if inside return
   * 0
   *
   * @param p the point
   * @param b the bounds
   * @return the squared distance
   */
  friend decltype(auto) distanceSquared(const Point<T, S> &p,
                                        const BoundsBase &b) {
    return max(Vector<T, S>{}, max(b.pMin - p, p - b.pMax)).squaredNorm();
  }

  /**
   * @brief minimum distance between point and bounds, if inside return 0
   *
   * @param p the point
   * @param b the bounds
   * @return the distance
   */
  friend decltype(auto) distance(const Point<T, S> &p, const BoundsBase &b) {
    return std::sqrt(distanceSquared(p, b));
  }

  /**
   * @brief get the bounding sphere of the bounds
   *
   * @param [out] center the center of the sphere
   * @param [out] rad the radius of the sphere
   */
  void boundingSphere(Point<T, S> &center, float &rad) {
    center = (pMin + pMax) / 2;
    rad = inside(center, *this) ? distance(center, pMax) : 0;
  }

  /**
   * @brief take the union of the two bounds
   *
   */
  friend BoundsBase operator|(const BoundsBase &a, const BoundsBase &b) {
    BoundsBase ret;
    ret.pMin = min(a.pMin, b.pMin);
    ret.pMax = max(a.pMax, b.pMax);
    return ret;
  }

  /**
   * @brief take the union between bounds and a point
   *
   */
  friend BoundsBase operator|(const BoundsBase &b, const Point<T, S> &p) {
    BoundsBase ret;
    ret.pMin = min(b.pMin, p);
    ret.pMax = max(b.Pmax, p);
    return ret;
  }

  /**
   * @brief take the intersect between two bounds
   *
   */
  friend BoundsBase operator&(const BoundsBase &a, const BoundsBase &b) {
    BoundsBase ret;
    // important: assign to pMin/pMax directly and don't run the BoundsBase()
    // constructor, since it takes min/max of the points passed to it.  In
    // turn, that breaks returning an invalid bound for the case where we
    // intersect non-overlapping bounds (as we'd like to happen).
    ret.pMin = max(a.pMin, b.pMin);
    ret.pMax = min(a.pMax, b.pMax);
    return ret;
  }

  [[nodiscard]] std::string toString() const {
    return "[" + pMin.toString() + ", " + pMax.toString() + "]";
  }

  friend std::ostream &operator<<(std::ostream &os, const BoundsBase &b) {
    return os << b.toString();
  }

  /// left bottom corner of the bounds
  Point<T, S> pMin{std::numeric_limits<T>::max()};
  /// right top corner of the bounds
  Point<T, S> pMax{std::numeric_limits<T>::lowest()};
};

/**
 * @brief 2d bounding box
 *
 */
template <ArithmeticType T>
class Bounds2 : public BoundsBase<T, 2> {
 public:
  using BoundsBase<T, 2>::BoundsBase;

  Bounds2(const BoundsBase<T, 2> &base) : BoundsBase<T, 2>(base) {}

  /**
   * @brief get the area of bounds
   *
   * @return the area
   */
  [[nodiscard]] decltype(auto) area() const {
    auto d = this->diagonal();
    return d.x() * d.y();
  }
};

/// 2d float bounds
using Bounds2f = Bounds2<float>;
/// 2d int bounds
using Bounds2i = Bounds2<int>;

/**
 * @brief 3d bounding box
 *
 */
template <ArithmeticType T>
class Bounds3 : public BoundsBase<T, 3> {
 public:
  using BoundsBase<T, 3>::BoundsBase;

  Bounds3(const BoundsBase<T, 3> &base) : BoundsBase<T, 3>(base) {}
};

/// 3d float bounds
using Bounds3f = Bounds3<float>;

/**
 * @brief 2d integer bounds iterator, iterate all pixels inside
 *
 */
class Bounds2iIterator : public std::forward_iterator_tag {
 public:
  explicit Bounds2iIterator(const Bounds2i &b, Point2i p)
      : b(b), p(std::move(p)) {}

  Bounds2iIterator operator++() {
    advance();
    return *this;
  }

  Bounds2iIterator operator++(int) {
    Bounds2iIterator ret = *this;
    advance();
    return ret;
  }

  bool operator==(const Bounds2iIterator &rhs) const {
    return p == rhs.p && (&b == &rhs.b);
  }

  bool operator!=(const Bounds2iIterator &rhs) const {
    return p != rhs.p || (&b != &rhs.b);
  }

  Point2i operator*() const { return p; }

 private:
  void advance() {
    ++p.x();
    if (p.x() == b.pMax.x()) {
      p.x() = b.pMin.x();
      ++p.y();
    }
  }

  const Bounds2i &b;
  Point2i p;
};

/**
 * @brief begin of the bounds2i iteration
 *
 */
inline Bounds2iIterator begin(const Bounds2i &b) {
  return Bounds2iIterator{b, b.pMin};
}

/**
 * @brief end of the bounds2i iteration
 *
 */
inline Bounds2iIterator end(const Bounds2i &b) {
  // normally, the ending point is at the minimum x value and one past
  // the last valid y value.
  Point2i pEnd(b.pMin.x(), b.pMax.y());
  // however, if the bounds are degenerate, override the end point to
  // equal the start point so that any attempt to iterate over the bounds
  // exits out immediately.
  if (b.pMin.x() >= b.pMax.x() || b.pMin.y() >= b.pMax.y()) pEnd = b.pMin;
  return Bounds2iIterator{b, pEnd};
}
}  // namespace dakku
#endif