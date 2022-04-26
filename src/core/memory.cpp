#include <core/memory.h>

namespace dakku {

MemoryArena &GlobalMemoryArena::instance() {
  static GlobalMemoryArena _instance;
  return _instance.arena;
}
}  // namespace dakku