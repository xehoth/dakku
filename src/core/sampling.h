#ifndef DAKKU_SRC_CORE_SAMPLING_H_
#define DAKKU_SRC_CORE_SAMPLING_H_
#include <core/vector.h>

DAKKU_BEGIN

/**
 * @param u 2d random number
 * @return the barycentric coordinate of the sampled point
 */
Point2f uniformSampleTriangle(const Point2f &u);

/**
 * sample disk
 * @param u 2d random number
 */
Point2f concentricSampleDisk(const Point2f &u);

/**
 * cosine weighted sample hemisphere
 * @param u 2d random number
 * @return the sampled w in the hemisphere
 */
inline Vector3f cosineSampleHemisphere(const Point2f &u) {
  Point2f d = concentricSampleDisk(u);
  Float z = std::sqrt(
      std::max(static_cast<Float>(0), 1 - d.x() * d.x() - d.y() * d.y()));
  return Vector3f(d.x(), d.y(), z);
}

DAKKU_END
#endif  // DAKKU_SRC_CORE_SAMPLING_H_
