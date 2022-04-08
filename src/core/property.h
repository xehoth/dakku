#ifndef DAKKU_CORE_PROPERTY_H_
#define DAKKU_CORE_PROPERTY_H_
#include <core/vector.h>
#include <core/transform.h>
#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <variant>

namespace dakku {
/**
 * @brief property type
 *
 */
enum class PropertyType {
  NONE,
  ARRAY,
  OBJECT,
  NUMBER,
  STRING,
  VECTOR,
  TRANSFORM
};

/**
 * @brief property class, containing many data types, similar to json
 *
 */
class DAKKU_EXPORT_CORE Property {
 public:
  using ObjectType = std::map<std::string, Property, std::less<>>;
  using ArrayType = std::vector<Property>;
  using NumberType = float;
  using VectorType = std::vector<float>;
  using StringType = std::string;
  using TransformType = Transform;

  /**
   * @brief Construct a new empty Property object
   *
   */
  Property() = default;

  /**
   * @brief Construct a new Property object with a number
   *
   * @param v number value
   */
  template <ArithmeticType T>
  Property(const T &v)
      : data(static_cast<float>(v)), type(PropertyType::NUMBER) {}

  Property(const std::string &s) : data(s), type(PropertyType::STRING) {}

  /**
   * @brief Construct a new Property object with a vector
   *
   * @param v vector
   */
  template <ArithmeticType T, size_t S, typename D>
  Property(const VectorBase<T, S, D> &v)
      : data(v.toFloatVector()), type(PropertyType::VECTOR) {}

  /**
   * @brief Construct a new Property object with a float vector
   *
   */
  Property(const std::vector<float> &v) {
    if (v.size() <= 3) {
      data = v;
      type = PropertyType::VECTOR;
    } else if (v.size() == 16) {
      data = Transform{v};
      type = PropertyType::TRANSFORM;
    } else {
      std::vector<Property> tmp;
      tmp.reserve(v.size());
      for (float i : v) tmp.emplace_back(i);
      data = std::move(tmp);
      type = PropertyType::ARRAY;
    }
  }

  /**
   * @brief Construct a new Property object with a transform
   *
   * @param t transform
   */
  Property(const Transform &t) : data(t), type(PropertyType::TRANSFORM) {}

  /**
   * @brief get the type of current property
   *
   * @return the property type
   */
  [[nodiscard]] decltype(auto) getPropertyType() const { return type; }

  /**
   * @brief get the property data according to given type
   *
   * @tparam type the type to get
   * @return the data
   */
  template <PropertyType type>
  [[nodiscard]] decltype(auto) get() {
    auto &ret = static_cast<const Property &>(*this).get<type>();
    return const_cast<std::add_lvalue_reference_t<std::decay_t<decltype(ret)>>>(
        ret);
  }

  /**
   * @brief get the property data according to given type (const)
   *
   * @tparam type the type to get
   * @return the data
   */
  template <PropertyType type, bool required = false>
  [[nodiscard]] decltype(auto) get() const {
    if constexpr (type == PropertyType::OBJECT) {
      return std::get<ObjectType>(data);
    } else if constexpr (type == PropertyType::ARRAY) {
      return std::get<ArrayType>(data);
    } else if constexpr (type == PropertyType::NUMBER) {
      return std::get<NumberType>(data);
    } else if constexpr (type == PropertyType::STRING) {
      return std::get<StringType>(data);
    } else if constexpr (type == PropertyType::VECTOR) {
      return std::get<VectorType>(data);
    } else if constexpr (type == PropertyType::TRANSFORM) {
      return std::get<TransformType>(data);
    } else {
      if constexpr (type == PropertyType::NONE) {
        DAKKU_WARN("none property type");
      } else {
        DAKKU_WARN("invalid property type: {}", type);
      }
      return std::get<std::nullptr_t>(data);
    }
  }

  /**
   * @brief get object
   *
   */
  [[nodiscard]] decltype(auto) getObject() {
    return get<PropertyType::OBJECT>();
  }

  /**
   * @brief get object
   *
   */
  [[nodiscard]] decltype(auto) getObject() const {
    return get<PropertyType::OBJECT>();
  }

  /**
   * @brief get array
   *
   */
  [[nodiscard]] decltype(auto) getArray() { return get<PropertyType::ARRAY>(); }

  /**
   * @brief get array
   *
   */
  [[nodiscard]] decltype(auto) getArray() const {
    return get<PropertyType::ARRAY>();
  }

  /**
   * @brief get number
   *
   */
  [[nodiscard]] decltype(auto) getNumber() {
    return get<PropertyType::NUMBER>();
  }

  /**
   * @brief get number
   *
   */
  [[nodiscard]] decltype(auto) getNumber() const {
    return get<PropertyType::NUMBER>();
  }

  /**
   * @brief get string
   *
   */
  [[nodiscard]] decltype(auto) getString() {
    return get<PropertyType::STRING>();
  }

  /**
   * @brief get string
   *
   */
  [[nodiscard]] decltype(auto) getString() const {
    return get<PropertyType::STRING>();
  }

  /**
   * @brief get vector
   *
   */
  [[nodiscard]] decltype(auto) getVector() {
    return get<PropertyType::VECTOR>();
  }

  /**
   * @brief get vector
   *
   */
  [[nodiscard]] decltype(auto) getVector() const {
    return get<PropertyType::VECTOR>();
  }

  /**
   * @brief get transform
   *
   */
  [[nodiscard]] decltype(auto) getTransform() {
    return get<PropertyType::TRANSFORM>();
  }

  /**
   * @brief get transform
   *
   */
  [[nodiscard]] decltype(auto) getTransform() const {
    return get<PropertyType::TRANSFORM>();
  }

  /**
   * @brief is this an object
   *
   */
  [[nodiscard]] bool isObjectType() const {
    return type == PropertyType::OBJECT;
  }

  /**
   * @brief is this an array
   *
   */
  [[nodiscard]] bool isArrayType() const { return type == PropertyType::ARRAY; }

  /**
   * @brief is this a number
   *
   */
  [[nodiscard]] bool isNumberType() const {
    return type == PropertyType::NUMBER;
  }

  /**
   * @brief is this a string
   *
   */
  [[nodiscard]] bool isStringType() const {
    return type == PropertyType::STRING;
  }

  /**
   * @brief is this a vector
   *
   */
  [[nodiscard]] bool isVectorType() const {
    return type == PropertyType::VECTOR;
  }

  /**
   * @brief is this a transform
   *
   */
  [[nodiscard]] bool isTransformType() const {
    return type == PropertyType::TRANSFORM;
  }

  /**
   * @brief index `key`, if this is NONE, then => OBJECT
   *
   * @param key key to index
   * @return value
   */
  decltype(auto) operator[](const std::string &key) {
    [[unlikely]] if (type == PropertyType::NONE) {
      data = ObjectType{};
      type = PropertyType::OBJECT;
    }
    DAKKU_CHECK(isObjectType(), "looking up {} in a non-object property", key);
    return getObject()[key];
  }

  /**
   * @brief index `key` (const), key must exists
   *
   * @param key key to index
   * @return value
   */
  decltype(auto) operator[](const std::string &key) const {
    [[unlikely]] if (!isObjectType()) {
      DAKKU_ERR("try to access {} in a non-object type property", key);
      std::exit(-1);
    }
    [[likely]] if (auto it = getObject().find(key); it != getObject().end()) {
      return it->second;
    }
    else {
      DAKKU_ERR("cannot find the request key: {}", key);
      std::exit(-1);
    }
  }

  /**
   * @brief index i, if this is NONE, then => ARRAY
   * if i >= size, it will grow automatically
   * @param i the index
   * @return the data
   */
  decltype(auto) operator[](size_t i) {
    [[unlikely]] if (type == PropertyType::NONE) {
      data = ArrayType{};
      type = PropertyType::ARRAY;
      getArray().reserve((i + 1) * 2);
    }
    DAKKU_CHECK(isArrayType(), "index {} in a non-array property", i);
    auto &arr = getArray();
    if (i >= arr.size()) {
      arr.reserve((i + 1) * 2);
      arr.resize(i + 1);
    }
    return getArray()[i];
  }

  /**
   * @brief index i, if this is NONE, then => ARRAY
   * if i >= size, it will grow automatically
   * @param i the index
   * @return the data
   */
  decltype(auto) operator[](size_t i) const {
    [[unlikely]] if (type == PropertyType::NONE) {
      DAKKU_ERR("try to index {} in a non-array type property", i);
      std::exit(-1);
    }
    [[likely]] if (const auto &arr = getArray(); i < arr.size()) {
      return arr[i];
    }
    else {
      DAKKU_ERR("index out of range: {} >= {}", i, arr.size());
      std::exit(-1);
    }
  }

  /**
   * @brief to string
   *
   */
  [[nodiscard]] std::string toString() const;

  /**
   * @brief output the property
   *
   */
  friend std::ostream &operator<<(std::ostream &os, const Property &property) {
    return os << property.toString();
  }

  friend void to_json(nlohmann::json &j, const Property &p) {
    switch (p.type) {
      case PropertyType::OBJECT: {
        for (const auto &[k, v] : p.getObject()) {
          to_json(j[k], v);
        }
        break;
      }
      case PropertyType::ARRAY: {
        j = nlohmann::json::array();
        for (const auto &v : p.getArray()) {
          j.push_back(nlohmann::json{});
          to_json(j.back(), v);
        }
        break;
      }
      case PropertyType::NUMBER: {
        j = p.getNumber();
        break;
      }
      case PropertyType::STRING: {
        j = p.getString();
        break;
      }
      case PropertyType::VECTOR: {
        j = p.getVector();
        break;
      }
      case PropertyType::TRANSFORM: {
        j = p.getTransform().getMatrix().toFloatVector();
        break;
      }
      case PropertyType::NONE: {
        DAKKU_ERR("none property type");
        break;
      }
      default: {
        DAKKU_ERR("invalid property type: {}", p.type);
        break;
      }
    }
  }

  friend void from_json(const nlohmann::json &j, Property &p) {
    if (j.is_object()) {
      for (auto it = j.begin(); it != j.end(); ++it)
        from_json(*it, p[it.key()]);
    } else if (j.is_array()) {
      auto isFloatVector = [&] {
        for (const auto &v : j)
          if (!v.is_number()) return false;
        return true;
      };
      if (isFloatVector()) {
        VectorType tmp;
        j.get_to(tmp);
        p = tmp;
      } else {
        for (size_t i = 0; i < j.size(); ++i) from_json(j[i], p[i]);
      }
    } else if (j.is_number()) {
      p = j.get<float>();
    } else if (j.is_string()) {
      p = j.get<std::string>();
    } else {
      DAKKU_ERR("unknown type in json: {}", j.type_name());
      std::exit(-1);
    }
  }

  /**
   * @brief special case, merge transforms (e.g., lookat, rotate, translate)
   *
   * @return Property&
   */
  [[nodiscard]] Transform mergeTransform() const {
    // already a transform
    Transform ret;
    if (isTransformType()) return ret;
    if (isArrayType()) {
      auto &arr = getArray();
      for (const auto &v : arr) {
        if (!v.isTransformType()) {
          if (v.isObjectType()) {
            std::string transType;
            if (auto it = v.getObject().find("type");
                it != v.getObject().end() && it->second.isStringType()) {
              transType = it->second.getString();
            } else {
              DAKKU_ERR("cannot merge transforms for: {}", *this);
            }
            if (transType == "lookat") {
              Point3f pos = v["pos"].getVector();
              Point3f look = v["look"].getVector();
              Vector3f refUp = v["up"].getVector();
              ret = ret * lookAt(pos, look, refUp);
            } else if (transType == "rotate") {
              float angle = v["data"].getNumber();
              if (v["axis"].isStringType()) {
                std::string axis = v["axis"].getString();
                if (axis == "x") {
                  ret = ret * rotateX(angle);
                } else if (axis == "y") {
                  ret = ret * rotateY(angle);
                } else if (axis == "z") {
                  ret = ret * rotateZ(angle);
                } else {
                  DAKKU_ERR("invalid axis for rotation: {}", axis);
                  return ret;
                }
              } else {
                Vector3f axis = v["axis"].getVector();
                ret = ret * rotate(angle, axis);
              }
            } else if (transType == "translate") {
              Vector3f delta = v["data"].getVector();
              ret = ret * translate(delta);
            } else if (transType == "scale") {
              Vector3f s = v["data"].getVector();
              ret = ret * scale(s.x(), s.y(), s.z());
            } else {
              DAKKU_ERR("invalid transform type: {}", type);
              return ret;
            }
          } else {
            DAKKU_ERR("cannot merge transforms for: {}", *this);
            return ret;
          }
        }
      }
    } else {
      DAKKU_ERR("cannot merge transforms for: {}", *this);
    }
    return ret;
  }

 private:
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// property data
  std::variant<std::nullptr_t, ObjectType, ArrayType, NumberType, StringType,
               VectorType, TransformType>
      data;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
  /// property type
  PropertyType type{PropertyType::NONE};
};
}  // namespace dakku
#endif