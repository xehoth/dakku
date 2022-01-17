#include <gtest/gtest.h>
#include <core/geometry.h>

using namespace dakku::core;

TEST(core, Vector) {
  Vector2f vec2f;
  EXPECT_EQ(vec2f.x(), 0);
  EXPECT_EQ(vec2f.y(), 0);
  vec2f = Vector2f(0.5f, -0.5f);
  EXPECT_EQ(vec2f.x(), 0.5f);
  EXPECT_EQ(vec2f.y(), -0.5f);
  Vector3f vec3f;
  EXPECT_EQ(vec3f.x(), 0);
  EXPECT_EQ(vec3f.y(), 0);
  EXPECT_EQ(vec3f.z(), 0);
  vec3f = Vector3f(0.5f, -0.5f, 0.7f);
  EXPECT_EQ(vec3f.x(), 0.5f);
  EXPECT_EQ(vec3f.y(), -0.5f);
  EXPECT_EQ(vec3f.z(), 0.7f);
  Vector2i v2i(3, -1);
  EXPECT_EQ(v2i.x(), 3);
  EXPECT_EQ(v2i.y(), -1);
  Vector3i v3i(3, -1, 4);
  EXPECT_EQ(v3i.x(), 3);
  EXPECT_EQ(v3i.y(), -1);
  EXPECT_EQ(v3i.z(), 4);
  vec2f = vec2f + vec2f;
  EXPECT_EQ(vec2f.x(), 1.0f);
  EXPECT_EQ(vec2f.y(), -1.0f);
  vec2f += vec2f;
  EXPECT_EQ(vec2f.x(), 2.0f);
  EXPECT_EQ(vec2f.y(), -2.0f);
  {
    auto a = Vector2f(1, -3);
    auto b = Vector2f(-2, 3);
    auto c = a - b;
    EXPECT_EQ(c.x(), 3.0f);
    EXPECT_EQ(c.y(), -6.0f);
    c = a;
    c -= b;
    EXPECT_EQ(c.x(), 3.0f);
    EXPECT_EQ(c.y(), -6.0f);
    c = a * 2;
    EXPECT_EQ(c.x(), 2.0f);
    EXPECT_EQ(c.y(), -6.0f);
    c = a;
    c *= 2;
    EXPECT_EQ(c.x(), 2.0f);
    EXPECT_EQ(c.y(), -6.0f);
    c = a / 2;
    EXPECT_EQ(c.x(), 0.5f);
    EXPECT_EQ(c.y(), -1.5f);
    c = a;
    c /= 2;
    EXPECT_EQ(c.x(), 0.5f);
    EXPECT_EQ(c.y(), -1.5f);
    c = -a;
    EXPECT_EQ(c.x(), -1.0f);
    EXPECT_EQ(c.y(), 3.0f);
  }
  {
    auto a = Vector3f(2, -1, 3);
    auto b = Vector3f(-3, 2, 4);
    auto c = a + b;
    EXPECT_EQ(c.x(), -1.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), 7.0f);
    c = a;
    c += b;
    EXPECT_EQ(c.x(), -1.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), 7.0f);
    c = a - b;
    EXPECT_EQ(c.x(), 5.0f);
    EXPECT_EQ(c.y(), -3.0f);
    EXPECT_EQ(c.z(), -1.0f);
    c = a;
    c -= b;
    EXPECT_EQ(c.x(), 5.0f);
    EXPECT_EQ(c.y(), -3.0f);
    EXPECT_EQ(c.z(), -1.0f);
    c = -a;
    EXPECT_EQ(c.x(), -2.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), -3.0f);
  }
  {
    auto a = Vector2f(Point2f(1.5, -2.5));
    EXPECT_EQ(a.x(), 1.5f);
    EXPECT_EQ(a.y(), -2.5f);
    auto b = Vector2f(Point3f(1.5f, -3.0f, 4.0f));
    EXPECT_EQ(b.x(), 1.5f);
    EXPECT_EQ(b.y(), -3.0f);
  }
}

TEST(core, Point) {
  {
    Point2f p2f;
    EXPECT_EQ(p2f.x(), 0);
    EXPECT_EQ(p2f.y(), 0);
    p2f = Point2f(-1, 2);
    EXPECT_EQ(p2f.x(), -1);
    EXPECT_EQ(p2f.y(), 2);
    p2f = Point2f(Point3f(3.0, -2.5, 4.0));
    EXPECT_EQ(p2f.x(), 3.0f);
    EXPECT_EQ(p2f.y(), -2.5f);
    p2f = Point2f(Vector2i(3, -4));
    EXPECT_EQ(p2f.x(), 3.0f);
    EXPECT_EQ(p2f.y(), -4.0f);
    auto b = Vector2i(p2f);
    EXPECT_EQ(b.x(), 3);
    EXPECT_EQ(b.y(), -4);
  }
  {
    auto a = Point2f(1.5f, 0.6f);
    Point2f b = a + Vector2f(1.3f, 1.0f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
  }
  {
    Vector2f v = Point2f(1.5f, 0.5f) - Point2f(0.5f, -1.0f);
    EXPECT_EQ(v.x(), 1.0f);
    EXPECT_EQ(v.y(), 1.5f);
    Point2f p = Point2f(1.5f, 0.5f) - Vector2f(0.5f, -1.0f);
    EXPECT_EQ(p.x(), 1.0f);
    EXPECT_EQ(p.y(), 1.5f);
    p = Point2f(1.5f, 0.5f);
    p -= Vector2f(0.5f, -1.0f);
    EXPECT_EQ(p.x(), 1.0f);
    EXPECT_EQ(p.y(), 1.5f);
  }
  {
    auto a = Point2f(1, -1);
    auto b = -a;
    EXPECT_EQ(b.x(), -1.0f);
    EXPECT_EQ(b.y(), 1.0f);
  }
  {
    auto a = Point2f(1.5f, 0.6f);
    Point2f b = a + Point2f(1.3f, 1.0f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
    b = a;
    b += Point2f(1.3f, 1.0f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
  }
  {
    auto a = Point2f(1.5f, 0.5f);
    a *= 2;
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), 1.0f);
    a = Point2f(1.5f, 0.5f);
    auto b = a * 2;
    a = b;
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), 1.0f);
    a = Point2f(1.5f, 0.5f);
    a /= 2;
    EXPECT_EQ(a, Point2f(1.5f, 0.5f) / 2);
    EXPECT_EQ(a.x(), 0.75f);
  }
  {
    Point3f a;
    EXPECT_EQ(a.x(), 0);
    EXPECT_EQ(a.y(), 0);
    EXPECT_EQ(a.z(), 0);
    a = Point3f(-1, 2, 3);
    EXPECT_EQ(a.x(), -1);
    EXPECT_EQ(a.y(), 2);
    EXPECT_EQ(a.z(), 3);
    a = Point3f(Point3i(3, -2, 4));
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), -2.0f);
    EXPECT_EQ(a.z(), 4.0f);
    a = Point3f(Vector3i(3, -4, 2));
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), -4.0f);
    EXPECT_EQ(a.z(), 2.0f);
    auto b = Vector2i(a);
    EXPECT_EQ(b.x(), 3);
    EXPECT_EQ(b.y(), -4);
  }
  {
    auto a = Point3f(1.5f, 0.6f, 0.5f);
    Point3f b = a + Vector3f(1.3f, 1.0f, 0.5f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
    EXPECT_EQ(b.z(), 1.0f);
  }
  {
    Vector3f v = Point3f(1.5f, 0.5f, 0.5f) - Point3f(0.5f, -1.0f, -0.5f);
    EXPECT_EQ(v.x(), 1.0f);
    EXPECT_EQ(v.y(), 1.5f);
    EXPECT_EQ(v.z(), 1.0f);
    Point3f p = Point3f(1.5f, 0.5f, 0.5f) - Vector3f(0.5f, -1.0f, -0.5f);
    EXPECT_EQ(p.x(), 1.0f);
    EXPECT_EQ(p.y(), 1.5f);
    EXPECT_EQ(p.z(), 1.0f);
    p = Point3f(1.5f, 0.5f, 0.5f);
    p -= Vector3f(0.5f, -1.0f, -0.5f);
    EXPECT_EQ(p.x(), 1.0f);
    EXPECT_EQ(p.y(), 1.5f);
    EXPECT_EQ(p.z(), 1.0f);
  }
  {
    auto a = Point3f(1, -1, 1);
    auto b = -a;
    EXPECT_EQ(b.x(), -1.0f);
    EXPECT_EQ(b.y(), 1.0f);
    EXPECT_EQ(b.z(), -1.0f);
  }
  {
    auto a = Point3f(1.5f, 0.6f, 1.0f);
    Point3f b = a + Point3f(1.3f, 1.0f, 1.0f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
    EXPECT_EQ(b.z(), 2.0f);
    b = a;
    b += Point3f(1.3f, 1.0f, 1.0f);
    EXPECT_EQ(b.x(), 2.8f);
    EXPECT_EQ(b.y(), 1.6f);
    EXPECT_EQ(b.z(), 2.0f);
  }
  {
    auto a = Point3f(1.5f, 0.5f, 1.0f);
    a *= 2;
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), 1.0f);
    EXPECT_EQ(a.z(), 2.0f);

    a = Point3f(1.5f, 0.5f, 1.0f);
    auto b = a * 2;
    a = b;
    EXPECT_EQ(a.x(), 3.0f);
    EXPECT_EQ(a.y(), 1.0f);
    EXPECT_EQ(a.z(), 2.0f);

    a = Point3f(1.5f, 0.5f, 1.0f);
    a /= 2;
    EXPECT_EQ(a, Point3f(1.5f, 0.5f, 1.0f) / 2);
    EXPECT_EQ(a.x(), 0.75f);
  }
}

TEST(core, Normal) {
  {
    Normal3f n;
    EXPECT_EQ(n.x(), 0);
    EXPECT_EQ(n.y(), 0);
    EXPECT_EQ(n.z(), 0);
    n = Normal3f(1, -1, 2);
    EXPECT_EQ(n.x(), 1);
    EXPECT_EQ(n.y(), -1);
    EXPECT_EQ(n.z(), 2);
    n = Normal3f(Vector3f(1, -1, 2));
    EXPECT_EQ(n.x(), 1);
    EXPECT_EQ(n.y(), -1);
    EXPECT_EQ(n.z(), 2);
    n = Normal3f(Point3f(1, -1, 2));
    EXPECT_EQ(n.x(), 1);
    EXPECT_EQ(n.y(), -1);
    EXPECT_EQ(n.z(), 2);
    n = Normal3f(Point3i(1, -1, 2));
    EXPECT_EQ(n.x(), 1);
    EXPECT_EQ(n.y(), -1);
    EXPECT_EQ(n.z(), 2);
    n = Normal3f(Vector3i(1, -1, 2));
    EXPECT_EQ(n.x(), 1);
    EXPECT_EQ(n.y(), -1);
    EXPECT_EQ(n.z(), 2);
    n = -n;
    EXPECT_EQ(n.x(), -1);
    EXPECT_EQ(n.y(), 1);
    EXPECT_EQ(n.z(), -2);
  }
  {
    auto a = Normal3f(2, -1, 3);
    auto b = Normal3f(-3, 2, 4);
    auto c = a + b;
    EXPECT_EQ(c.x(), -1.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), 7.0f);
    c = a;
    c += b;
    EXPECT_EQ(c.x(), -1.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), 7.0f);
    c = a - b;
    EXPECT_EQ(c.x(), 5.0f);
    EXPECT_EQ(c.y(), -3.0f);
    EXPECT_EQ(c.z(), -1.0f);
    c = a;
    c -= b;
    EXPECT_EQ(c.x(), 5.0f);
    EXPECT_EQ(c.y(), -3.0f);
    EXPECT_EQ(c.z(), -1.0f);
    c = -a;
    EXPECT_EQ(c.x(), -2.0f);
    EXPECT_EQ(c.y(), 1.0f);
    EXPECT_EQ(c.z(), -3.0f);
  }
}