#ifndef DAKKU_MATH_FWD_H_
#define DAKKU_MATH_FWD_H_
#include <platform.h>
#include <type_traits>
#include <limits>
#include <numbers>

/**
 * @brief dakku namespace
 */
namespace dakku {

#if DAKKU_BUILD_MODULE != DAKKU_MATH_MODULE
#define DAKKU_EXPORT_MATH DAKKU_IMPORT
#else
#define DAKKU_EXPORT_MATH DAKKU_EXPORT
#endif

/// infinity
static constexpr float INF = std::numeric_limits<float>::infinity();

/**
 * @brief concept: `T` is an arithemetic type
 * 
 * @tparam T typename
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;


DAKKU_EXPORT_MATH void testMath();
}  // namespace dakku
#endif