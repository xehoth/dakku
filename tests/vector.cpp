#include <gtest/gtest.h>
#include <math/vector.h>
#include <iostream>
using namespace dakku;

TEST(Math, Vector) {
  float b[3]{1.5, -2.3, 3.4};
  Vector3f a(b[0], b[1], b[2]);
  auto eq = [&] { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2]; };
  EXPECT_TRUE(eq());
  a = 1 / -((9 * (4 - (1 + a + 2) * 3 - 1)) / 4);
  for (int i = 0; i < 3; ++i)
    b[i] = 1 / -(9 * (4 - (1 + b[i] + 2) * 3 - 1) / 4);
  EXPECT_TRUE(eq());
}