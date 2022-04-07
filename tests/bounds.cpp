#include <gtest/gtest.h>
#include <core/bounds.h>

using namespace dakku;

// some test are adopted from pbrt-v3

TEST(Bounds, IteratorBasic) {
  Bounds2i b{{0, 1}, {2, 3}};
  Point2i e[]{{0, 1}, {1, 1}, {0, 2}, {1, 2}};
  int offset = 0;
  for (const auto &p : b) {
    EXPECT_LT(offset, sizeof(e) / sizeof(e[0]));
    EXPECT_EQ(e[offset], p) << "offset = " << offset;
    ++offset;
  }
}

TEST(Bounds2, IteratorDegenerate) {
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