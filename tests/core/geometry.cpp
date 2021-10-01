//
// Created by xehoth on 2021/10/1.
//
#include <gtest/gtest.h>
#include <dakku/core/geometry.h>

using namespace dakku;

TEST(Ray, Ray) {
  Ray r(Point3f(0, -2, 3), Vector3f(2, -3, 4), 2);
  EXPECT_FLOAT_EQ(r.o.x, 0);
  EXPECT_FLOAT_EQ(r.o.y, -2);
  EXPECT_FLOAT_EQ(r.o.z, 3);
  Float len2 = 2 * 2 + 3 * 3 + 4 * 4;
  Float len = std::sqrt(len2);
  EXPECT_FLOAT_EQ(r.d.x, 2 / len);
  EXPECT_FLOAT_EQ(r.d.y, -3 / len);
  EXPECT_FLOAT_EQ(r.d.z, 4 / len);
  EXPECT_FLOAT_EQ(r.tMax, 2);
}

TEST(Ray, at) {
  Ray r(Point3f(0, -2, 3), Vector3f(2, -3, 4), 2);
  Float len2 = 2 * 2 + 3 * 3 + 4 * 4;
  Float len = std::sqrt(len2);

  Point3f p = r(0.5);
  EXPECT_FLOAT_EQ(p.x, 0 + static_cast<Float>(2 * 0.5) / len);
  EXPECT_FLOAT_EQ(p.y, -2 - static_cast<Float>(3 * 0.5) / len);
  EXPECT_FLOAT_EQ(p.z, 3 + static_cast<Float>(4 * 0.5) / len);
}
