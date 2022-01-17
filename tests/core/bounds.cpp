#include <gtest/gtest.h>
#include <core/bounds.h>

using namespace dakku::core;

TEST(core, Bounds) {
  {
    Bounds2f b;
    EXPECT_EQ(b.p_min, Point2f(std::numeric_limits<float>::lowest(),
                               std::numeric_limits<float>::lowest()));
  }
  {
    Bounds2f b(Point2f(1, 3), Point2f(2, -1));
    EXPECT_EQ(b.p_min, Point2f(1, -1));
    EXPECT_EQ(b.p_max, Point2f(2, 3));
    LOG_INFO("dis: {}", distance_squared(b.p_min, b.p_max));
  }
}