#include <core/property.h>

namespace dakku {

std::string Property::toString() const {
  std::string ret;
  switch (type) {
    case PropertyType::OBJECT: {
      ret += "{";
      for (const auto &[k, v] : std::get<ObjectType>(data)) {
        ret += "\"";
        ret += k;
        ret += "\":";
        ret += v.toString();
        ret += ",";
      }
      if (ret.back() == ',') ret.pop_back();
      ret += "}";
      break;
    }
    case PropertyType::ARRAY: {
      ret += "[";
      for (const auto &v : std::get<ArrayType>(data)) {
        ret += v.toString();
        ret += ",";
      }
      if (ret.back() == ',') ret.pop_back();
      ret += "]";
      break;
    }
    case PropertyType::NUMBER: {
      ret = std::to_string(std::get<NumberType>(data));
      break;
    }
    case PropertyType::VECTOR: {
      ret += "[";
      for (const auto &v : std::get<VectorType>(data)) {
        ret += std::to_string(v);
        ret += ",";
      }
      if (ret.back() == ',') ret.pop_back();
      ret += "]";
      break;
    }
    case PropertyType::NONE: {
      DAKKU_ERR("none property type");
      break;
    }
    default: {
      DAKKU_ERR("invalid property type: {}", type);
      break;
    }
  }
  return ret;
}

}  // namespace dakku