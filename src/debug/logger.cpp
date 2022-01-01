#include "edl/logger.h"

#define SPDLOG_ACTIVE_LEVEL 0
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace edl {

const uint32_t levelMap[] = {
	spdlog::level::trace,
	spdlog::level::debug,
	spdlog::level::info,
	spdlog::level::warn,
	spdlog::level::err,
	spdlog::level::critical,
	spdlog::level::off
};

void log::setLevel(const Level& level) {
	spdlog::set_level(static_cast<spdlog::level::level_enum>(levelMap[static_cast<uint32_t>(level)]));
}

void log::setLevel(const std::string& name, const Level& level) { 
	spdlog::get(name).get()->set_level(static_cast<spdlog::level::level_enum>(levelMap[static_cast<uint32_t>(level)]));
}

void log::logger(const std::string& name) {
	spdlog::stdout_color_mt(name, spdlog::color_mode::always);
}

void log::logger(const std::string& name, const std::string& filename) {
	spdlog::basic_logger_mt(name, filename);
}

void log::trace(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::trace(msg);
	spdlog::get(name).get()->trace(msg);
}
void log::debug(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::debug(msg);
	spdlog::get(name).get()->debug(msg);
}
void log::info(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::info(msg);
	spdlog::get(name).get()->info(msg);
}
void log::warn(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::warn(msg);
	spdlog::get(name).get()->warn(msg);
}
void log::error(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::error(msg);
	spdlog::get(name).get()->error(msg);
}
void log::critical(const std::string& name, const std::string& msg) {
	if (name == "") spdlog::critical(msg);
	spdlog::get(name).get()->critical(msg);
}

}