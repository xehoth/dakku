#ifndef DAKKU_CORE_PROPERTY_H_
#define DAKKU_CORE_PROPERTY_H_
#include <core/vector.h>
#include <map>
#include <string>
#include <variant>

namespace dakku {
/**
 * @brief property type
 *
 */
enum class PropertyType { NONE, ARRAY, OBJECT, NUMBER, VECTOR };

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

  /**
   * @brief Construct a new Property object with a vector
   *
   * @param v vector
   */
  template <ArithmeticType T, size_t S, typename D>
  Property(const VectorBase<T, S, D> &v)
      : data(v.toFloatVector()), type(PropertyType::VECTOR) {}

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
    } else if constexpr (type == PropertyType::VECTOR) {
      return std::get<VectorType>(data);
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
   * @brief is this a vector
   *
   */
  [[nodiscard]] bool isVectorType() const {
    return type == PropertyType::VECTOR;
  }

  /**
   * @brief index `key`, if this is NONE, then => OBJECT
   *
   * @param key key to index
   * @return value
   */
  decltype(auto) operator[](const std::string &key) {
    if (type == PropertyType::NONE) [[unlikely]] {
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
    if (!isObjectType()) [[unlikely]] {
      DAKKU_ERR("try to access {} in a non-object type property", key);
      std::exit(-1);
    }
    if (auto it = getObject().find(key); it != getObject().end()) [[likely]] {
      return it->second;
    } else {
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
    if (type == PropertyType::NONE) [[unlikely]] {
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
    if (type == PropertyType::NONE) [[unlikely]] {
      DAKKU_ERR("try to index {} in a non-array type property", i);
      std::exit(-1);
    }
    if (const auto &arr = getArray(); i < arr.size()) [[likely]] {
      return arr[i];
    } else {
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

 private:
  /// property data
  std::variant<ObjectType, ArrayType, NumberType, VectorType, std::nullptr_t>
      data;
  /// property type
  PropertyType type{PropertyType::NONE};
};
}  // namespace dakku
#endif