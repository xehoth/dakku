#include <gtest/gtest.h>
#include <core/ray.h>

using namespace dakku::core;

TEST(core, Ray) {
  {
    Ray r;
    EXPECT_EQ(r.o, Point3f());
    EXPECT_EQ(r.d, Vector3f());
    EXPECT_TRUE(std::isinf(r.t_max));
  }
  {
    Point3f o(1, 2, 3);
    Vector3f dir(1, -2, 3);
    Ray r(o, dir, 5.0f);
    EXPECT_EQ(r.o, o);
    EXPECT_EQ(r.d, dir.normalized());
    EXPECT_EQ(r.t_max, 5.0f);
    EXPECT_EQ(r(3.5f), o + Vector3f(dir.normalized()) * 3.5f);
  }
}

TEST(core, RayDifferential) {
  {
    RayDifferential r;
    EXPECT_EQ(r.o, Point3f());
    EXPECT_EQ(r.d, Vector3f());
    EXPECT_TRUE(std::isinf(r.t_max));
    EXPECT_EQ(r.has_differentials, false);
    EXPECT_EQ(r.rx_origin, Point3f());
    EXPECT_EQ(r.ry_origin, Point3f());
    EXPECT_EQ(r.rx_direction, Vector3f());
    EXPECT_EQ(r.ry_direction, Vector3f());
  }
}
