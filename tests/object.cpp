#include <gtest/gtest.h>
#include <core/api.h>
#include <stream/api.h>
#include <filters/api.h>
#include <textures/api.h>

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
  EXPECT_TRUE(Class::instance().contains("Texture"));
  EXPECT_TRUE(Class::instance().contains("ImageTexture"));
  EXPECT_TRUE(Class::instance().contains("ConstantTexture"));
  EXPECT_TRUE(Class::instance().contains("TextureMapping2D"));
  EXPECT_TRUE(Class::instance().contains("UvMapping2D"));
  EXPECT_TRUE(Class::instance().contains("Shape"));
}