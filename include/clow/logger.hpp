#pragma once

#include <string_view>

namespace clow {

/// Log levels used for console output.
enum class LogLevel {
    Info,
    Warning,
    Error,
    Progress
};

/// Console logger with colored output and structured progress reporting.
class Logger {
public:
    static void info(std::string_view message) noexcept;
    static void warning(std::string_view message) noexcept;
    static void error(std::string_view message) noexcept;
    static void progress(std::string_view message) noexcept;
};

} // namespace clow
