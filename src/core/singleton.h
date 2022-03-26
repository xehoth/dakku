#ifndef DAKKU_SRC_CORE_SINGLETON_H_
#define DAKKU_SRC_CORE_SINGLETON_H_
#include <core/fwd.h>

DAKKU_BEGIN
/**
 * dakku singleton pattern
 * @tparam T type
 */
template <typename T>
class Singleton {
 public:
  /**
   * get the singleton instance
   */
  static T &instance();
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

 protected:
  explicit Singleton() = default;
};

template <typename T>
T &Singleton<T>::instance() {
  static T _inst;
  return _inst;
}

DAKKU_END
#endif  // DAKKU_SRC_CORE_SINGLETON_H_
