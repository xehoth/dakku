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
  explicit BoundsBase(const Point<T, S> &p) : p_min(p), p_max(p) {}

  /**
   * @brief type conversion
   *
   */
  template <ArithmeticType U>
  explicit BoundsBase(const BoundsBase<U, S> &b)
      : p_min(Point<T, S>(b.p_min)), p_max(Point<T, S>(b.p_max)) {}

  /**
   * @brief Construct a new Bounds Base object with two points
   * (take min/max of them to pMin and pMax)
   *
   */
  BoundsBase(const Point<T, S> &p1, const Point<T, S> &p2)
      : p_min(min(p1, p2)), p_max(max(p1, p2)) {}

  BoundsBase(const sol::table &table)
      : p_min(table.get_or(1, table)), p_max(table.get_or(2, table)) {}

  /**
   * @brief get the diagonal of the bounding box
   *
   */
  [[nodiscard]] Vector<T, S> diagonal() const { return p_max - p_min; }

  /**
   * @brief get the dimension who has the maximum extent
   *
   * @return the dimension
   */
  [[nodiscard]] size_t max_extent() const {
    return this->diagonal().max_element_index();
  }

  bool operator==(const BoundsBase &rhs) const {
    return p_min == rhs.p_min && p_max == rhs.p_max;
  }

  bool operator!=(const BoundsBase &rhs) const {
    return p_min != rhs.p_min || p_max != rhs.p_max;
  }

  /**
   * @brief linera interpolation between pMin and pMax
   *
   * @param t interpolate $t$ (element wise)
   */
  [[nodiscard]] Point<T, S> lerp(const Point<float, S> &t) const {
    Point<T, S> ret;
    for (size_t i = 0; i < S; ++i) ret[i] = std::lerp(p_min[i], p_max[i], t[i]);
    return ret;
  }

  /**
   * @brief get the offset $v$ of a point, p = o + v * d
   *
   * @param p the point
   * @return the offset
   */
  [[nodiscard]] Vector<T, S> offset(const Point<T, S> &p) const {
    Vector<T, S> o = p - p_min;
    for (size_t i = 0; i < S; ++i)
      if (p_max[i] > p_min[i]) o[i] /= p_max[i] - p_min[i];
    return o;
  }

  /**
   * @brief check whether two bounds overlaps
   *
   */
  friend bool overlaps(const BoundsBase &a, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(a.p_max[i] >= b.p_min[i] && a.p_min[i] <= b.p_max[i])) return false;
    return true;
  }

  /**
   * @brief check whether two bounds overlaps
   *
   */
  [[nodiscard]] bool overlaps(const BoundsBase &rhs) const {
    return overlaps(*this, rhs);
  }

  /**
   * @brief check whether point is inside the bounds (inclusive)
   *
   * @param p the point
   * @param b the bounds
   */
  friend bool inside(const Point<T, S> &p, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(p[i] >= b.p_min[i] && p[i] <= b.p_max[i])) return false;
    return true;
  }

  /**
   * @brief check whether point is inside the bounds (inclusive)
   *
   * @param p the point
   */
  [[nodiscard]] bool inside(const Point<T, S> &p) const {
    return inside(p, *this);
  }

  /**
   * @brief check whether point is inside the bounds (exclusive)
   *
   * @param p the point
   * @param b the bounds
   */
  friend bool inside_exclusive(const Point<T, S> &p, const BoundsBase &b) {
    for (size_t i = 0; i < S; ++i)
      if (!(p[i] >= b.p_min[i] && p[i] < b.p_max[i])) return false;
    return true;
  }

  /**
   * @brief check whether point is inside the bounds (exclusive)
   *
   * @param p the point
   */
  [[nodiscard]] bool inside_exclusive(const Point<T, S> &p) const {
    return inside_exclusive(p, *this);
  }

  /**
   * @brief get the bounding sphere of the bounds
   *
   * @param [out] center the center of the sphere
   * @param [out] rad the radius of the sphere
   */
  void bounding_sphere(Point<T, S> &center, float &rad) {
    center = (p_min + p_max) / 2;
    rad = inside(center, *this) ? distance(center, p_max) : 0;
  }

  /**
   * @brief take the union of the two bounds
   *
   */
  friend BoundsBase operator|(const BoundsBase &a, const BoundsBase &b) {
    BoundsBase ret;
    ret.p_min = min(a.p_min, b.p_min);
    ret.p_max = max(a.p_max, b.p_max);
    return ret;
  }

  /**
   * @brief take the union of the two bounds
   *
   */
  [[nodiscard]] BoundsBase union_bounds(const BoundsBase &rhs) const {
    return *this | rhs;
  }

  /**
   * @brief take the union between bounds and a point
   *
   */
  friend BoundsBase operator|(const BoundsBase &b, const Point<T, S> &p) {
    BoundsBase ret;
    ret.p_min = min(b.p_min, p);
    ret.p_max = max(b.p_max, p);
    return ret;
  }

  /**
   * @brief take the union between bounds and a point
   *
   */
  [[nodiscard]] BoundsBase union_bounds(const Point<T, S> &p) const {
    return *this | p;
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
    ret.p_min = max(a.p_min, b.p_min);
    ret.p_max = min(a.p_max, b.p_max);
    return ret;
  }

  /**
   * @brief take the intersect between two bounds
   *
   */
  [[nodiscard]] BoundsBase intersect(const BoundsBase &rhs) const {
    return *this & rhs;
  }

  /**
   * @brief minimum squared distance between point and bounds, if inside return
   * 0
   *
   * @param p the point
   * @param b the bounds
   * @return the squared distance
   */
  friend decltype(auto) distance_squared(const Point<T, S> &p,
                                         const BoundsBase<T, S> &b) {
    return max(Vector<T, S>{}, max(b.p_min - p, p - b.p_max)).squared_norm();
  }

  /**
   * @brief minimum distance between point and bounds, if inside return 0
   *
   * @param p the point
   * @param b the bounds
   * @return the distance
   */
  friend decltype(auto) distance(const Point<T, S> &p,
                                 const BoundsBase<T, S> &b) {
    return std::sqrt(distance_squared(p, b));
  }

  [[nodiscard]] std::string to_string() const {
    return "[" + p_min.to_string() + ", " + p_max.to_string() + "]";
  }

  friend std::ostream &operator<<(std::ostream &os, const BoundsBase &b) {
    return os << b.to_string();
  }

  /// left bottom corner of the bounds
  Point<T, S> p_min{std::numeric_limits<T>::max()};
  /// right top corner of the bounds
  Point<T, S> p_max{std::numeric_limits<T>::lowest()};
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
    if (p.x() == b.p_max.x()) {
      p.x() = b.p_min.x();
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
  return Bounds2iIterator{b, b.p_min};
}

/**
 * @brief end of the bounds2i iteration
 *
 */
inline Bounds2iIterator end(const Bounds2i &b) {
  // normally, the ending point is at the minimum x value and one past
  // the last valid y value.
  Point2i pEnd(b.p_min.x(), b.p_max.y());
  // however, if the bounds are degenerate, override the end point to
  // equal the start point so that any attempt to iterate over the bounds
  // exits out immediately.
  if (b.p_min.x() >= b.p_max.x() || b.p_min.y() >= b.p_max.y()) pEnd = b.p_min;
  return Bounds2iIterator{b, pEnd};
}

DAKKU_DECLARE_LUA_OBJECT(Bounds, DAKKU_EXPORT_CORE);
}  // namespace dakku
#endif