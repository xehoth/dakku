#ifndef DAKKU_PLATFORM_H_
#define DAKKU_PLATFORM_H_
#if defined(_MSC_VER)
#if !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#endif

#if !defined(DAKKU_BEGIN)
/// dakku namespace begin
#define DAKKU_BEGIN namespace dakku {
#endif

#if !defined(DAKKU_END)
#define DAKKU_END }
#endif

DAKKU_BEGIN

#if defined(_MSC_VER)
#define DAKKU_INLINE __forceinline
#define DAKKU_EXPORT __declspec(dllexport)
#define DAKKU_IMPORT __declspec(dllimport)
#else
#define DAKKU_INLINE __attribute__((always_inline)) inline
#define DAKKU_EXPORT __attribute__((visibility("default")))
#define DAKKU_IMPORT
#endif

#define DAKKU_MATH_MODULE 1
#define DAKKU_CORE_MODULE 2
#define DAKKU_MAIN_MODULE 10

/*! @mainpage dakku documentation
This is the documentation of dakku renderer.

![cornell_box](cornell_box.png)
*/

DAKKU_END
#endif