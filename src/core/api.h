#ifndef DAKKU_SRC_CORE_API_H_
#define DAKKU_SRC_CORE_API_H_
#include <core/fwd.h>
#include <core/logger.h>
#include <core/class.h>
#include <core/object.h>
#include <core/film.h>
#include <core/state.h>

DAKKU_BEGIN
namespace core {
/**
 * initialize dakku::core
 */
static void init() {
  logger::init("log.txt");
//  logger::init();
  DAKKU_REGISTER_CLASS(Object);
  DAKKU_REGISTER_CLASS(Film);
  DAKKU_REGISTER_CLASS(RenderState);
}
}  // namespace core
DAKKU_END
#endif  // DAKKU_SRC_CORE_API_H_
