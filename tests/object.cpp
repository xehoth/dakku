#include <gtest/gtest.h>
#include <core/api.h>
#include <stream/api.h>

using namespace dakku;

TEST(Object, Register) {
  EXPECT_TRUE(Class::instance().contains("Object"));
  EXPECT_TRUE(Class::instance().contains("SerializableObject"));
  EXPECT_TRUE(Class::instance().contains("InputStream"));
  EXPECT_TRUE(Class::instance().contains("OutputStream"));
  EXPECT_TRUE(Class::instance().contains("FileInputStream"));
  EXPECT_TRUE(Class::instance().contains("FileOutputStream"));
}