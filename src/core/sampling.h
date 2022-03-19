#ifndef DAKKU_SRC_CORE_SAMPLING_H_
#define DAKKU_SRC_CORE_SAMPLING_H_
#include <core/vector.h>

DAKKU_BEGIN

/**
 * @param u 2d random number
 * @return the barycentric coordinate of the sampled point
 */
Point2f uniformSampleTriangle(const Point2f &u);

DAKKU_END
#endif  // DAKKU_SRC_CORE_SAMPLING_H_
