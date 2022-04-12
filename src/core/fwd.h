#ifndef DAKKU_CORE_FWD_H_
#define DAKKU_CORE_FWD_H_
#include <platform.h>
#include <type_traits>
#include <limits>
#include <numbers>
#include <cmath>
#include <cstdint>
#include <bit>

namespace dakku {

#if DAKKU_BUILD_MODULE != DAKKU_CORE_MODULE
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#endif

/// infinity
static constexpr float INF = std::numeric_limits<float>::infinity();

/// PI
static constexpr float PI = std::numbers::pi_v<float>;

/// L1 cache line size
static constexpr size_t L1_CACHE_LINE_SIZE = 64;

/// shadow epsilon, used for shadow ray, offset ray origin
static constexpr float SHADOW_EPS = 1e-4f;
/// 1 - eps
static constexpr float ONE_MINUS_EPSILON =
    1 - std::numeric_limits<float>::epsilon();

/**
 * @brief concept: `T` is an arithemetic type
 *
 * @tparam T typename
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (floating point)
 * @param x given value
 * @return whether `x` is nan
 */
template <typename T>
requires std::is_floating_point_v<T> DAKKU_INLINE bool isNaN(T x) {
  return std::isnan(x);
}

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (integer)
 * @return false (all integers are not nans)
 */
template <typename T>
requires std::is_integral_v<T> DAKKU_INLINE bool isNaN(T) { return false; }

/**
 * @brief convert degrees to radians
 *
 * @param deg given degree
 * @return the radian of deg
 */
inline float radians(float deg) { return PI / 180.0f * deg; }

/**
 * @brief convert radians to degrees
 *
 * @param rad the given radian
 * @return the degree of rad
 */
inline float degrees(float rad) { return 180.0f / PI * rad; }

/**
 * @brief check whether `v` is power of 2
 *
 * @tparam T integer type
 * @param v value
 */
template <typename T>
requires std::is_integral_v<T>
inline constexpr bool isPowerOf2(T v) { return v && !(v & (v - 1)); }

/**
 * @brief round up to nearest value which is power of 2
 *
 */
inline std::int32_t roundUpPow2(std::int32_t v) {
  --v;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  return v + 1;
}

inline float evalLanczos(float x, float tau = 2) {
  x = std::abs(x);
  if (x < 1e-5f) return 1;
  if (x > 1.0f) return 0;
  x *= PI;
  float s = std::sin(x * tau) / (x * tau);
  float lanczos = std::sin(x) / x;
  return s * lanczos;
}

/**
 * @brief $\lfloor \log_2 v \rfloor$
 *
 */
inline int log2Int(std::int32_t v) {
  return 31 - std::countl_zero(static_cast<std::uint32_t>(v));
}

/**
 * @brief $\lfloor \log_2 v \rfloor$
 *
 */
inline int log2Int(std::int64_t v) {
  return 63 - std::countl_zero(static_cast<std::uint64_t>(v));
}

/**
 * @brief linear interpolation
 *
 */
template <ArithmeticType T>
inline T lerp(T a, T b, T t) {
  return std::lerp(a, b, t);
}

class DAKKU_EXPORT_CORE RelativeRoot;
class DAKKU_EXPORT_CORE InputStream;
class DAKKU_EXPORT_CORE OutputStream;

enum class PropertyType;
class DAKKU_EXPORT_CORE Property;
class DAKKU_EXPORT_CORE Class;
class DAKKU_EXPORT_CORE Object;
struct DAKKU_EXPORT_CORE Matrix4x4;
/**
 * @brief object type concept
 *
 */
template <typename T>
concept ObjectType = std::is_base_of_v<Object, T>;
class Transform;
class Ray;
class RayDifferential;

class DAKKU_EXPORT_CORE Filter;
class DAKKU_EXPORT_CORE Film;
class FilmTile;
template <size_t S, typename Derived>
class CoefficientSpectrum;
class RgbSpectrum;
/// Spectrum type: use RgbSpectrum
using Spectrum = RgbSpectrum;
struct Interaction;
class SurfaceInteraction;
class DAKKU_EXPORT_CORE Shape;
class DAKKU_EXPORT_CORE Texture;

class DAKKU_EXPORT_CORE Camera;
struct CameraSample;
}  // namespace dakku
#endif