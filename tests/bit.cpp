#include <gtest/gtest.h>
#include <core/fwd.h>

using namespace dakku;

TEST(Bit, Log2Int) {
  for (int i = 0; i < 32; ++i) {
    uint32_t ui = 1u << i;
    EXPECT_EQ(i, log2Int(ui));
    EXPECT_EQ(i, log2Int((uint64_t)ui));
  }

  for (int i = 1; i < 31; ++i) {
    uint32_t ui = 1u << i;
    EXPECT_EQ(i, log2Int(ui + 1));
    EXPECT_EQ(i, log2Int((uint64_t)(ui + 1)));
  }

  for (int i = 0; i < 64; ++i) {
    uint64_t ui = 1ull << i;
    EXPECT_EQ(i, log2Int(ui));
  }

  for (int i = 1; i < 64; ++i) {
    uint64_t ui = 1ull << i;
    EXPECT_EQ(i, log2Int(ui + 1));
  }
}

TEST(Bit, IsPowerOf2) {
  for (int i = 0; i < 32; ++i) {
    uint32_t ui = 1u << i;
    EXPECT_EQ(true, isPowerOf2(ui));
    if (ui > 1) {
      EXPECT_EQ(false, isPowerOf2(ui + 1));
    }
    if (ui > 2) {
      EXPECT_EQ(false, isPowerOf2(ui - 1));
    }
  }
}

TEST(Bit, roundUpPow2) {
  EXPECT_EQ(roundUpPow2(7), 8);
  for (int i = 1; i < (1 << 24); ++i)
    if (isPowerOf2(i))
      EXPECT_EQ(roundUpPow2(i), i);
    else
      EXPECT_EQ(roundUpPow2(i), 1 << (log2Int(i) + 1));

  for (int64_t i = 1; i < (1 << 24); ++i)
    if (isPowerOf2(i))
      EXPECT_EQ(roundUpPow2(i), i);
    else
      EXPECT_EQ(roundUpPow2(i), 1 << (log2Int(i) + 1));

  for (int i = 0; i < 30; ++i) {
    int v = 1 << i;
    EXPECT_EQ(roundUpPow2(v), v);
    if (v > 2) EXPECT_EQ(roundUpPow2(v - 1), v);
    EXPECT_EQ(roundUpPow2(v + 1), 2 * v);
  }

  for (int i = 0; i < 62; ++i) {
    int64_t v = 1ll << i;
    EXPECT_EQ(roundUpPow2(v), v);
    if (v > 2) EXPECT_EQ(roundUpPow2(v - 1), v);
    EXPECT_EQ(roundUpPow2(v + 1), 2 * v);
  }
}
