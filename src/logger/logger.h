#pragma once
#include <spdlog/spdlog.h>

namespace dakku {

void init_logger(std::string_view filename = {});

#define LOG_ERROR SPDLOG_ERROR
#define LOG_INFO SPDLOG_INFO
#define LOG_WARN SPDLOG_WARN
#define LOG_DEBUG SPDLOG_DEBUG

#ifdef _DEBUG
#define DAKKU_CHECK(val, ...) \
  do {                        \
    if (!(val)) {             \
      LOG_ERROR(__VA_ARGS__); \
    }                         \
  } while (0)
#else
#define DAKKU_CHECK(...) static_cast<void>(0)
#endif

}  // namespace dakku
