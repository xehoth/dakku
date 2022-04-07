#include <gtest/gtest.h>
#include <core/object.h>

using namespace dakku;

TEST(Object, Register) {
  EXPECT_TRUE(Class::instance().contains("Object"));
}