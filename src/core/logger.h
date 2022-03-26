#ifndef DAKKU_SRC_CORE_LOGGER_H_
#define DAKKU_SRC_CORE_LOGGER_H_
#include <core/fwd.h>
#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <string_view>

DAKKU_BEGIN

#define DAKKU_ERR SPDLOG_ERROR
#define DAKKU_INFO SPDLOG_INFO
#define DAKKU_WARN SPDLOG_WARN
#define DAKKU_DEBUG SPDLOG_DEBUG

namespace core::logger {
/**
 * init logger
 * @param fileName write log into the file
 */
void init(std::string_view fileName = {});
}  // namespace core::logger

#ifdef _DEBUG
#define DAKKU_CHECK(val, ...) \
  do {                        \
    if (!(val)) {             \
      DAKKU_ERR(__VA_ARGS__); \
    }                         \
  } while (0)
#else
#define DAKKU_CHECK(...) static_cast<void>(0)
#endif

DAKKU_END
#endif  // DAKKU_SRC_CORE_LOGGER_H_
