#include <gtest/gtest.h>
#include <math/vector.h>
#include <iostream>
using namespace dakku;

TEST(Math, Vector) {
  Vector3f a(1, 2, 3);
  float b[3]{1, 2, 3};
  auto eq = [&] { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2]; };
  EXPECT_TRUE(eq());
  // a = 6 - (4 - (1 + a + 1) * 3 - 1) * 9;
  a = (1 + a + 2);
  // std::cerr << a << std::endl;
  for (int i = 0; i < 3; ++i) b[i] = (1 + b[i] + 2);
  // for (int i = 0; i < 3; ++i) std::cerr << b[i] << " ";
  EXPECT_TRUE(eq());
  // a = -a;
  // for (int i = 0; i < 3; ++i) b[i] = -b[i];
  // EXPECT_TRUE(eq());
}