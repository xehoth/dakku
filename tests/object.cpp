#include <gtest/gtest.h>
#include <core/api.h>
#include <stream/api.h>
#include <filters/api.h>

using namespace dakku;

TEST(Object, Register) {
  EXPECT_TRUE(Class::instance().contains("Object"));
  EXPECT_TRUE(Class::instance().contains("SerializableObject"));
  EXPECT_TRUE(Class::instance().contains("InputStream"));
  EXPECT_TRUE(Class::instance().contains("OutputStream"));
  EXPECT_TRUE(Class::instance().contains("FileInputStream"));
  EXPECT_TRUE(Class::instance().contains("FileOutputStream"));
  EXPECT_TRUE(Class::instance().contains("Filter"));
  EXPECT_TRUE(Class::instance().contains("BoxFilter"));
  EXPECT_TRUE(Class::instance().contains("TriangleFilter"));
}