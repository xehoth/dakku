#include <gtest/gtest.h>
#include <core/property.h>

using namespace dakku;

TEST(Property, Parse) {
  Vector3f a(1, 2, 3);
  Point3f b(1, 2, 3);

  Property p;
  p[0] = a;
  p[1] = 2;
  p[2] = {};
  Matrix4x4 m;
  Transform trans(m);
  p[2]["test"] = trans;

  auto json = nlohmann::json::parse(p.toString());

  EXPECT_EQ(json.dump(), p.toString()) << json.dump() << "\n" << p.toString();

  p = json;

  EXPECT_EQ(p.toString(), json.dump()) << p.toString() << "\n" << json.dump();
}