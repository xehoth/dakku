#include <core/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dakku {

Logger::Logger(std::shared_ptr<spdlog::logger> logger)
    : _logger(std::move(logger)) {
#ifdef DAKKU_BUILD_DEBUG
  _logger->set_level(spdlog::level::debug);
#endif
}

const Logger &Logger::get() {
  static Logger logger{spdlog::stdout_color_mt("dakku")};
  return logger;
}
}  // namespace dakku