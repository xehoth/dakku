//
// Created by 82454 on 2022/3/14.
//

#ifndef DAKKU_SRC_CORE_VECTOR_BASE_H_
#define DAKKU_SRC_CORE_VECTOR_BASE_H_
#include <core/math.h>
#include <core/logger.h>
#include <core/serialization.h>
#include <Eigen/Dense>
#include <span>

DAKKU_BEGIN

template <ArithmeticType T, size_t size, typename Derived>
class VectorBase {
 public:
  using internalType = Eigen::Matrix<T, size, 1>;

  explicit VectorBase() { _data.setZero(); }

  template <ArithmeticType Arg>
  explicit VectorBase(Arg value) : _data(internalType::Constant(value)) {}

  template <ArithmeticType... Args>
  explicit VectorBase(Args &&...args) : _data(std::forward<Args>(args)...) {
    DAKKU_CHECK(!hasNaN(), "has nan");
  }

  template <typename OtherDerived>
  requires(!std::is_same_v<Derived, OtherDerived>) explicit VectorBase(
      const VectorBase<T, size, OtherDerived> &other)
      : _data(other._data) {
    DAKKU_CHECK(!hasNaN(), "has nan");
  }
  template <typename OtherDerived>
  requires(!std::is_same_v<Derived, OtherDerived>) explicit VectorBase(
      VectorBase<T, size, OtherDerived> &&other)
      : _data(std::move(other._data)) {}

  template <ArithmeticType, size_t, typename>
  friend class VectorBase;

  template <ArithmeticType U, typename OtherDerived>
  explicit VectorBase(const VectorBase<U, size, OtherDerived> &other)
      : _data(other._data.template cast<T>()) {}

  [[nodiscard]] bool hasNaN() const { return _data.hasNaN(); }

  VectorBase(const VectorBase &other) : _data(other._data) {
    DAKKU_CHECK(!hasNaN(), "has nan");
  }
  VectorBase(VectorBase &&other) : _data(std::move(other._data)) {
    DAKKU_CHECK(!hasNaN(), "has nan");
  }

  const Derived &derived() const { return static_cast<const Derived &>(*this); }
  Derived &derived() {
    return const_cast<Derived &>(
        static_cast<const VectorBase &>(*this).derived());
  }

  VectorBase &operator=(const VectorBase &other) {
    if (this == &other) return *this;
    _data = other._data;
    DAKKU_CHECK(!hasNaN(), "has nan");
    return *this;
  }
  VectorBase &operator=(VectorBase &&other) noexcept {
    if (this == &other) return *this;
    _data = std::move(other._data);
    return *this;
  }

  [[nodiscard]] bool operator==(const VectorBase &rhs) const {
    return _data == rhs._data;
  }
  [[nodiscard]] bool operator!=(const VectorBase &rhs) const {
    return _data != rhs._data;
  }

  template <ArithmeticType U>
  requires std::is_same_v<std::common_type_t<T, U>, T> Derived
  operator*(U f) const {
    return VectorBase(_data * static_cast<T>(f));
  }
  template <ArithmeticType U>
  requires std::is_same_v<std::common_type_t<T, U>, T> VectorBase &operator*=(
      U f) {
    DAKKU_CHECK(!isNaN(f), "has nan");
    _data *= static_cast<T>(f);
    return *this;
  }
  template <ArithmeticType U>
  requires std::is_same_v<std::common_type_t<T, U>, T>
  friend Derived operator*(U f, const VectorBase &rhs) {
    return rhs * static_cast<T>(f);
  }
  template <ArithmeticType U>
  requires std::is_same_v<std::common_type_t<T, U>, T> Derived
  operator/(U f) const {
    return VectorBase(_data / static_cast<T>(f));
  }
  template <ArithmeticType U>
  requires std::is_same_v<std::common_type_t<T, U>, T> VectorBase &operator/=(
      U f) {
    DAKKU_CHECK(f != 0, "f is zero");
    _data /= static_cast<T>(f);
    return *this;
  }
  Derived operator-() const { return VectorBase(-_data); }
  T operator[](int i) const {
    DAKKU_CHECK(0 <= i && i < size, "index out of range");
    return _data[i];
  }
  T &operator[](int i) {
    DAKKU_CHECK(0 <= i && i < size, "index out of range");
    return _data[i];
  }
  decltype(auto) x() const { return _data.x(); }
  decltype(auto) x() { return _data.x(); }
  decltype(auto) y() const { return _data.y(); }
  decltype(auto) y() { return _data.y(); }
  decltype(auto) z() const { return _data.z(); }
  decltype(auto) z() { return _data.z(); }
  decltype(auto) w() const { return _data.w(); }
  decltype(auto) w() { return _data.w(); }

  [[nodiscard]] bool isZero() const { return _data.isZero(); }

  [[nodiscard]] std::span<const T, size> span() const {
    return std::span<const T, size>{_data.data(), size};
  }
  [[nodiscard]] std::span<T, size> span() {
    return std::span<T, size>{_data.data(), size};
  }
  operator std::span<const T, size>() const { return span(); }
  operator std::span<T, size>() { return span(); }

  template <ArithmeticType U>
  Derived pow(U f) const {
    return VectorBase(_data.pow(f));
  }

  Derived normalized() const { return VectorBase(_data.normalized()); }
  VectorBase &normalize() {
    _data.normalize();
    return *this;
  }
  decltype(auto) squaredNorm() const { return _data.squaredNorm(); }
  decltype(auto) norm() const { return _data.norm(); }

  friend Derived min(const VectorBase &a, const VectorBase &b) {
    return VectorBase(a._data.cwiseMin(b._data));
  }
  friend Derived max(const VectorBase &a, const VectorBase &b) {
    return VectorBase(a._data.cwiseMax(b._data));
  }
  friend Derived floor(const VectorBase &v) {
    return VectorBase(v._data.array().floor());
  }
  friend Derived ceil(const VectorBase &v) {
    return VectorBase(v._data.array().ceil());
  }
  template <typename OtherDerived>
  decltype(auto) dot(const VectorBase &rhs) const {
    return _data.dot(rhs._data);
  }
  template <typename OtherDerived>
  decltype(auto) dot(const VectorBase<T, size, OtherDerived> &rhs) const {
    return _data.dot(rhs._data);
  }
  template <typename OtherDerived>
  decltype(auto) absDot(const VectorBase<T, size, OtherDerived> &rhs) const {
    return std::abs(dot(rhs));
  }
  Derived cross(const VectorBase &rhs) const {
    return VectorBase(_data.cross(rhs._data));
  }

  friend inline Derived barycentricInterpolate(const VectorBase &a,
                                                  const VectorBase &b,
                                                  const VectorBase &c,
                                                  const Point2f &u) {
    return b[0] * a + b[1] * b + (1 - b[0] - b[1]) * c;
  }

  friend void to_json(nlohmann::json &nlohmann_json_j,
                      const VectorBase &nlohmann_json_t) {
    nlohmann_json_j = nlohmann_json_t._data;
  }
  friend void from_json(const nlohmann::json &nlohmann_json_j,
                        VectorBase &nlohmann_json_t) {
    T tmp[size];
    nlohmann_json_j.get_to(tmp);
    nlohmann_json_t._data = Eigen::Map<internalType>(tmp, size, 1);
  }

  friend std::ostream &operator<<(std::ostream &os, const VectorBase &value) {
    os << '[';
    size_t i = 0;
    for (; i + 1 < size; ++i) os << value[i] << ", ";
    os << value[i] << ']';
    return os;
  }

  friend struct Matrix4x4;
  friend struct Transform;

 protected:
  explicit VectorBase(const internalType &data) : _data(data) {}
  explicit VectorBase(internalType &&data) : _data(std::move(data)) {}
  template <template <typename> typename EigenTmp, typename OtherDerived>
  requires(
      std::is_same_v<EigenTmp<OtherDerived>, Eigen::EigenBase<OtherDerived>> ||
      std::is_same_v<EigenTmp<OtherDerived>, Eigen::DenseBase<OtherDerived>> ||
      std::is_same_v<
          EigenTmp<OtherDerived>,
          Eigen::ReturnByValue<
              OtherDerived>>) explicit VectorBase(const EigenTmp<OtherDerived>
                                                      &data)
      : _data(data) {}

  Derived operator+(const VectorBase &rhs) const {
    return VectorBase(_data + rhs._data);
  }
  VectorBase &operator+=(const VectorBase &rhs) {
    DAKKU_CHECK(!rhs.hasNaN(), "has nan");
    _data += rhs._data;
    return *this;
  }
  Derived operator-(const VectorBase &rhs) const {
    return VectorBase(_data - rhs._data);
  }
  VectorBase &operator-=(const VectorBase &rhs) {
    DAKKU_CHECK(!rhs.hasNaN(), "has nan");
    _data -= rhs._data;
    return *this;
  }

  internalType _data;
};

#define DAKKU_DECLARE_VECTOR_DERIVED(type, T, size)     \
  using super = VectorBase<T, size, type<T, size>>;     \
  using VectorBase<T, size, type<T, size>>::VectorBase; \
  type(const super &base) : super(base) {}              \
  type(super &&base) : super(std::move(base)) {}

DAKKU_END
#endif  // DAKKU_SRC_CORE_VECTOR_BASE_H_
