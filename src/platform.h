#ifndef DAKKU_PLATFORM_H_
#define DAKKU_PLATFORM_H_
#if defined(_MSC_VER)
#if !defined(_USE_MATH_DEFINES) && !defined(M_PI)
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
/// dakku imageio module
#define DAKKU_IMAGEIO_MODULE 3
/// dakku filters module
#define DAKKU_FILTERS_MODULE 4
/// dakku textures module
#define DAKKU_TEXTURES_MODULE 5
/// dakku main module
#define DAKKU_MAIN_MODULE 10

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif

}  // namespace dakku

/*! @mainpage dakku documentation
This is the documentation of dakku renderer.

![cornell_box](cornell_box.png)

@subpage vector_base
*/
#endif