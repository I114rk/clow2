#include "clow/logger.hpp"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

namespace clow {

namespace {

constexpr const char* infoColor = "\x1b[32m";
constexpr const char* warningColor = "\x1b[33m";
constexpr const char* errorColor = "\x1b[31m";
constexpr const char* progressColor = "\x1b[36m";
constexpr const char* resetColor = "\x1b[0m";

/// Legacy cmd.exe prints escape sequences literally unless the console opts in.
bool colorsSupported() noexcept {
#ifdef _WIN32
    static const bool enabled = [] {
        const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        if (console == INVALID_HANDLE_VALUE || console == nullptr) {
            return false;
        }
        DWORD mode = 0;
        if (!GetConsoleMode(console, &mode)) {
            return false;
        }
        return SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }();
    return enabled;
#else
    return true;
#endif
}

void write(std::string_view prefix, std::string_view message, const char* color) noexcept {
    if (colorsSupported()) {
        std::cout << color << prefix << resetColor << " " << message << '\n';
    } else {
        std::cout << prefix << " " << message << '\n';
    }
}

} // namespace

void Logger::info(std::string_view message) noexcept {
    write("[INFO]", message, infoColor);
}

void Logger::warning(std::string_view message) noexcept {
    write("[WARN]", message, warningColor);
}

void Logger::error(std::string_view message) noexcept {
    write("[ERROR]", message, errorColor);
}

void Logger::progress(std::string_view message) noexcept {
    write("[PROG]", message, progressColor);
}

} // namespace clow
