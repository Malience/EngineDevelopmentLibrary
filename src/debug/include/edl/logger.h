#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace edl {

class log {
public:
    enum class Level : uint32_t {
        trace = 0,
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        critical = 5,
        off = 6
    };

    static void setLevel(const Level& level);
    static void setLevel(const std::string& name, const Level& level);
    static void logger(const std::string& name);
    static void logger(const std::string& name, const std::string& filename);

    static void trace(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void trace(const std::string& name, const FormatString& fmt, Args&&...args) { trace(name, format(fmt, std::forward<Args>(args)...)); }

    static void debug(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void debug(const std::string& name, const FormatString& fmt, Args&&...args) { debug(name, format(fmt, std::forward<Args>(args)...)); }

    static void info(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void info(const std::string& name, const FormatString& fmt, Args&&...args) { info(name, format(fmt, std::forward<Args>(args)...)); }

    static void warn(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void warn(const std::string& name, const FormatString& fmt, Args&&...args) { warn(name, format(fmt, std::forward<Args>(args)...)); }

    static void error(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void error(const std::string& name, const FormatString& fmt, Args&&...args) { error(name, format(fmt, std::forward<Args>(args)...)); }

    static void critical(const std::string& name, const std::string& msg);
    template<typename FormatString, typename... Args>
    static inline void critical(const std::string& name, const FormatString& fmt, Args&&...args) { critical(name, format(fmt, std::forward<Args>(args)...)); }

private:
    template<typename FormatString, typename... Args>
    static inline std::string format(const FormatString& fmt, Args&&...args) {
        std::string f = static_cast<std::string>(fmt);
        int size_s = std::snprintf(nullptr, 0, f.c_str(), args ...) + 1; // Extra space for '\0'
        if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
        auto size = static_cast<size_t>(size_s);
        auto buf = std::make_unique<char[]>(size);
        std::snprintf(buf.get(), size, f.c_str(), args ...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};

}