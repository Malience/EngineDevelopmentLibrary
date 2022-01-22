#include "edl/logger.h"

#include <chrono>
#include <iomanip>
#include <iostream>

namespace edl {
namespace log {

// Console Color Codes
const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

const std::string BRIGHTBLUE = "\033[94m";

const std::string CRITCOLOR = "\033[41;37m";

std::string levelMap[6] = {
	"trace",
	BRIGHTBLUE + "debug" + RESET,
	GREEN + "info" + RESET,
	YELLOW + "warn" + RESET,
	RED + "error" + RESET,
	CRITCOLOR + "crit" + RESET
};


void log(const std::string& name, const log::Level& level, const std::string& msg) {
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::cout << "[" << std::put_time(std::localtime(&time), "%F %T.") << std::setfill('0') << std::setw(3) << ms.count() << "] [" << name << "] [" << levelMap[(uint32_t)level] << "] " << msg << std::endl;
}

void log::setLevel(const Level& level) {
	//spdlog::set_level(static_cast<spdlog::level::level_enum>(levelMap[static_cast<uint32_t>(level)]));
}

void log::setLevel(const std::string& name, const Level& level) {
	//spdlog::get(name).get()->set_level(static_cast<spdlog::level::level_enum>(levelMap[static_cast<uint32_t>(level)]));
}

void log::logger(const std::string& name) {
	//spdlog::stdout_color_mt(name, spdlog::color_mode::always);
}

void log::logger(const std::string& name, const std::string& filename) {
	//spdlog::basic_logger_mt(name, filename);
}

void log::trace(const std::string& name, const std::string& msg) {
	log(name, Level::trace, msg);
}
void log::debug(const std::string& name, const std::string& msg) {
	log(name, Level::debug, msg);
}
void log::info(const std::string& name, const std::string& msg) {
	log(name, Level::info, msg);
}
void log::warn(const std::string& name, const std::string& msg) {
	log(name, Level::warn, msg);
}
void log::error(const std::string& name, const std::string& msg) {
	log(name, Level::error, msg);
}
void log::critical(const std::string& name, const std::string& msg) {
	log(name, Level::critical, msg);
}

}
}