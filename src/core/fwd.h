#ifndef DAKKU_CORE_FWD_H_
#define DAKKU_CORE_FWD_H_
#include <platform.h>

#include <type_traits>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_CORE_MODULE
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#endif

/**
 * @brief concept: `T` is an arithemetic type
 *
 * @tparam T typename
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;

}  // namespace dakku
#endif