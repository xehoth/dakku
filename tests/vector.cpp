#include <gtest/gtest.h>
#include <core/vector.h>

using namespace dakku;

TEST(Vector, Basic) {
  std::array<float, 3> b{1.5f, -2.3f, 3.4f};
  Vector3f a(b[0], b[1], b[2]);
  auto eq = [&] { return a[0] == b[0] && a[1] == b[1] && a[2] == b[2]; };
  EXPECT_TRUE(eq());
  a = 1 / -((9 * (4 - (1 + a + 2) * 3 - 1)) / 4);
  for (float &i : b) i = 1 / -(9 * (4 - (1 + i + 2) * 3 - 1) / 4);
  EXPECT_TRUE(eq());
}