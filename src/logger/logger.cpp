#include <logger/logger.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dakku::log {

void init(std::string_view filename) {
  spdlog::cfg::load_env_levels();
  if (!filename.empty()) {
    spdlog::set_default_logger(
        spdlog::basic_logger_mt("dakku", filename.data()));
  } else {
    spdlog::set_default_logger(spdlog::stdout_color_mt("dakku"));
  }
}
}  // namespace dakku::log