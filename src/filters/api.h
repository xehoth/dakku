#ifndef DAKKU_SRC_FILTERS_API_H_
#define DAKKU_SRC_FILTERS_API_H_
#include <core/class.h>
#include <filters/box.h>

DAKKU_BEGIN
namespace filters {
static void init() { DAKKU_REGISTER_CLASS(BoxFilter); }
}  // namespace filters
DAKKU_END
#endif  // DAKKU_SRC_FILTERS_API_H_
