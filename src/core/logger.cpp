#include <core/logger.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

DAKKU_BEGIN

namespace core::logger {
void init(std::string_view fileName) {
  spdlog::cfg::load_env_levels();
  if (!fileName.empty()) {
    spdlog::set_default_logger(
        spdlog::basic_logger_mt("dakku", fileName.data()));
  } else {
    spdlog::set_default_logger(spdlog::stdout_color_mt("dakku"));
  }
}
}  // namespace core::logger

DAKKU_END