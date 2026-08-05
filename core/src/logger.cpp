#include "clow/logger.hpp"
#include <iostream>

namespace clow {

namespace {

constexpr const char* infoColor = "\x1b[32m";
constexpr const char* warningColor = "\x1b[33m";
constexpr const char* errorColor = "\x1b[31m";
constexpr const char* progressColor = "\x1b[36m";
constexpr const char* resetColor = "\x1b[0m";

void write(std::string_view prefix, std::string_view message, const char* color) noexcept {
    std::cout << color << prefix << resetColor << " " << message << '\n';
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
