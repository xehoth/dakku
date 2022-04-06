#ifndef DAKKU_LOGGER_LOGGER_H_
#define DAKKU_LOGGER_LOGGER_H_
#include <logger/fwd.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <cstdlib>

namespace dakku {

/**
 * @brief logger class
 * 
 */
class DAKKU_EXPORT_LOGGER Logger {
 public:
  /**
   * @brief get logger instance
   *
   * @return the logger instance
   */
  static const Logger &get();

  /**
   * @brief get raw spdlog::logger handle
   *
   * @return the corresponding spdlog::logger
   */
  [[nodiscard]] spdlog::logger *handle() const { return _logger.get(); }

 private:
  /**
   * @brief Construct a new Logger object with spdlog::logger
   *
   * @param _logger given spdlog::logger
   */
  explicit Logger(std::shared_ptr<spdlog::logger> _logger);

  /// spdlog logger
  std::shared_ptr<spdlog::logger> _logger;
};

/// log error message
#define DAKKU_ERR(...) \
  SPDLOG_LOGGER_CALL(Logger::get().handle(), spdlog::level::err, __VA_ARGS__)
/// log info message
#define DAKKU_INFO(...) \
  SPDLOG_LOGGER_CALL(Logger::get().handle(), spdlog::level::info, __VA_ARGS__)
/// log warning message
#define DAKKU_WARN(...) \
  SPDLOG_LOGGER_CALL(Logger::get().handle(), spdlog::level::warn, __VA_ARGS__)
#ifdef DAKKU_BUILD_DEBUG
/// log debug message
#define DAKKU_DEBUG(...) \
  SPDLOG_LOGGER_CALL(Logger::get().handle(), spdlog::level::debug, __VA_ARGS__)
#else
/// log debug message
#define DAKKU_DEBUG(...) static_cast<void>(0)
#endif

#ifdef DAKKU_BUILD_CHECK
/// check whether the condition `val` is true, if not log error messages
#define DAKKU_CHECK(val, ...) \
  do {                        \
    if (!(val)) {             \
      DAKKU_ERR(__VA_ARGS__); \
      std::exit(-1);          \
    }                         \
  } while (0)
#else
/// check whether the condition `val` is true, if not log error messages
#define DAKKU_CHECK(...) static_cast<void>(0)
#endif

}  // namespace dakku
#endif