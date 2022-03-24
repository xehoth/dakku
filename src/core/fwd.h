#ifndef DAKKU_SRC_CORE_FWD_H_
#define DAKKU_SRC_CORE_FWD_H_
#include <defines.h>
#include <type_traits>
#include <limits>
#include <numbers>

#if !defined(DAKKU_BEGIN)
#define DAKKU_BEGIN namespace dakku {
#endif

#if !defined(DAKKU_END)
#define DAKKU_END }
#endif

DAKKU_BEGIN

#if !defined(DAKKU_EXPORT_CORE)
#if DAKKU_BUILD_MODULE == DAKKU_MODULE_CORE
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#endif
#endif

#ifdef DAKKU_FLOAT_AS_DOUBLE
using Float = double;
#else
using Float = float;
#endif

template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;

// global constants
static constexpr Float INF = std::numeric_limits<Float>::infinity();
static constexpr Float MAX_FLOAT = std::numeric_limits<Float>::max();
static constexpr Float SHADOW_EPS = static_cast<Float>(0.0001);
static constexpr Float PI = std::numbers::pi_v<Float>;
static constexpr Float INV_PI = std::numbers::inv_pi_v<Float>;
static constexpr Float PI_OVER_2 = PI / 2;
static constexpr Float PI_OVER_4 = PI / 4;
static constexpr size_t L1_CACHE_LINE_SIZE = 64;
static constexpr Float ONE_MINUS_EPSILON =
    1 - std::numeric_limits<Float>::epsilon();

// abstraction & helpers
template <typename T>
class Singleton;
class Object;
class Class;
template <typename T>
concept ObjectType = std::is_base_of_v<Object, T>;

// vectors
template <ArithmeticType T, size_t size, typename Derived>
class VectorBase;
template <ArithmeticType T, size_t size>
class Vector;
template <ArithmeticType T, size_t size>
class Normal;
template <ArithmeticType T, size_t size>
class Point;
template <ArithmeticType T>
using Vector2 = Vector<T, 2>;
template <ArithmeticType T>
using Vector3 = Vector<T, 3>;
template <ArithmeticType T>
using Point2 = Point<T, 2>;
template <ArithmeticType T>
using Point3 = Point<T, 3>;
template <ArithmeticType T>
using Normal3 = Normal<T, 3>;

using Vector2f = Vector2<Float>;
using Vector3f = Vector3<Float>;
using Point2i = Point2<int>;
using Point2f = Point2<Float>;
using Point3f = Point3<Float>;
using Normal3f = Normal3<Float>;

class Ray;
class RayDifferential;

// serialization
class InputStream;
class OutputStream;
class FileInputStream;
class FileOutputStream;
struct Serializable;
struct SerializableObject;
struct RelativeRoot;

// spectrum
template <ArithmeticType T, size_t size>
class CoefficientSpectrumT;
template <size_t size>
using CoefficientSpectrum = CoefficientSpectrumT<Float, size>;
class RgbSpectrum;

#ifdef DAKKU_SAMPLED_SPECTRUM

#else
using Spectrum = RgbSpectrum;
#endif

// transform
struct Matrix4x4;
class Transform;
class TransformCache;

class Film;
class FilmTile;
class Filter;

class Shape;
struct Interaction;
class SurfaceInteraction;
class VisibilityTester;
class Light;
class AreaLight;
class Primitive;
class Material;
class BxDF;
class BSDF;
template <typename T>
concept TextureDataType = std::is_same_v<T, Float> || std::is_same_v<T, Spectrum>;
template <TextureDataType T>
class Texture;
class Scene;
class RNG;
class Sampler;
class Integrator;
struct CameraSample;
class Camera;
class ProjectiveCamera;

class MemoryArena;
template <typename T>
class TypedMemoryArena;

class RenderState;
DAKKU_END
#endif  // DAKKU_SRC_CORE_FWD_H_