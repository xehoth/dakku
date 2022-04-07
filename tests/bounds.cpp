#include <gtest/gtest.h>
#include <core/bounds.h>

using namespace dakku;

// some test are adopted from pbrt-v3

TEST(Bounds, IteratorBasic) {
  Bounds2i b{{0, 1}, {2, 3}};
  std::vector<Point2i> e{{0, 1}, {1, 1}, {0, 2}, {1, 2}};
  int offset = 0;
  for (const auto &p : b) {
    EXPECT_LT(offset, e.size());
    EXPECT_EQ(e[offset], p) << "offset = " << offset;
    ++offset;
  }
}

TEST(Bounds, IteratorDegenerate) {
  Bounds2i b{{0, 0}, {0, 10}};
  for (const auto &p : b) {
    // this loop should never run.
    bool reached = true;
    EXPECT_FALSE(reached) << "p = " << p;
    break;
  }

  Bounds2i b2{{0, 0}, {4, 0}};
  for (const auto &p : b2) {
    // this loop should never run.
    bool reached = true;
    EXPECT_FALSE(reached) << "p = " << p;
    break;
  }

  Bounds2i b3;
  for (const auto &p : b3) {
    // this loop should never run.
    bool reached = true;
    EXPECT_FALSE(reached) << "p = " << p;
    break;
  }
}

TEST(Bounds, UnionBounds2) {
  Bounds2f a(Point2f(-10, -10), Point2f(0, 20));
  Bounds2f b;  // degenerate
  Bounds2f c = a | b;
  EXPECT_EQ(a, c);

  EXPECT_EQ(b, b | b);

  Bounds2f d(Point2f(-15, 10));
  Bounds2f e = a | d;
  EXPECT_EQ(Bounds2f(Point2f(-15, -10), Point2f(0, 20)), e);
}

TEST(Bounds, UnionBounds3) {
  Bounds3f a(Point3f(-10, -10, 5), Point3f(0, 20, 10));
  Bounds3f b;  // degenerate
  Bounds3f c = a | b;
  EXPECT_EQ(a, c);

  EXPECT_EQ(b, b | b);

  Bounds3f d(Point3f(-15, 10, 30));
  Bounds3f e = a | d;
  EXPECT_EQ(Bounds3f(Point3f(-15, -10, 5), Point3f(0, 20, 30)), e);
}

TEST(Bounds, PointDistance) {
  {
    Bounds3f b(Point3f(0, 0, 0), Point3f(1, 1, 1));

    // Points inside the bounding box or on faces
    EXPECT_EQ(0., distance(Point3f(.5, .5, .5), b));
    EXPECT_EQ(0., distance(Point3f(0, 1, 1), b));
    EXPECT_EQ(0., distance(Point3f(.25, .8, 1), b));
    EXPECT_EQ(0., distance(Point3f(0, .25, .8), b));
    EXPECT_EQ(0., distance(Point3f(.7, 0, .8), b));

    // Aligned with the plane of one of the faces
    EXPECT_EQ(5., distance(Point3f(6, 1, 1), b));
    EXPECT_EQ(10., distance(Point3f(0, -10, 1), b));

    // 2 of the dimensions inside the box's extent
    EXPECT_EQ(2., distance(Point3f(0.5, 0.5, 3), b));
    EXPECT_EQ(3., distance(Point3f(0.5, 0.5, -3), b));
    EXPECT_EQ(2., distance(Point3f(0.5, 3, 0.5), b));
    EXPECT_EQ(3., distance(Point3f(0.5, -3, 0.5), b));
    EXPECT_EQ(2., distance(Point3f(3, 0.5, 0.5), b));
    EXPECT_EQ(3., distance(Point3f(-3, 0.5, 0.5), b));

    // General points
    EXPECT_EQ(3 * 3 + 7 * 7 + 10 * 10, distanceSquared(Point3f(4, 8, -10), b));
    EXPECT_EQ(6 * 6 + 10 * 10 + 7 * 7, distanceSquared(Point3f(-6, -10, 8), b));
  }

  {
    // A few with a more irregular box, just to be sure
    Bounds3f b(Point3f(-1, -3, 5), Point3f(2, -2, 18));
    EXPECT_EQ(0., distance(Point3f(-.99, -2, 5), b));
    EXPECT_EQ(2 * 2 + 6 * 6 + 4 * 4, distanceSquared(Point3f(-3, -9, 22), b));
  }
}
