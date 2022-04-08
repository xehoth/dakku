#ifndef DAKKU_PLATFORM_H_
#define DAKKU_PLATFORM_H_
#if defined(_MSC_VER)
#if !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#endif

/**
 * @brief dakku namespace
 */
namespace dakku {

#if defined(_MSC_VER)
#define DAKKU_INLINE __forceinline
#define DAKKU_EXPORT __declspec(dllexport)
#define DAKKU_IMPORT __declspec(dllimport)
#else
#define DAKKU_INLINE __attribute__((always_inline)) inline
#define DAKKU_EXPORT __attribute__((visibility("default")))
#define DAKKU_IMPORT
#endif

/// dakku core module
#define DAKKU_CORE_MODULE 1
/// dakku stream module
#define DAKKU_STREAM_MODULE 2
/// dakku filters module
#define DAKKU_FILTERS_MODULE 2
/// dakku main module
#define DAKKU_MAIN_MODULE 10


}  // namespace dakku

/*! @mainpage dakku documentation
This is the documentation of dakku renderer.

![cornell_box](cornell_box.png)

@subpage vector_base
*/
#endif