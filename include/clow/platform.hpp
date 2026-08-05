#pragma once

#include <filesystem>
#include <string>

namespace clow {

namespace platform {

std::filesystem::path appDataPath();
std::filesystem::path executablePath();
bool ensureDirectory(const std::filesystem::path& directory) noexcept;
bool fileExists(const std::filesystem::path& filePath) noexcept;
std::string runProcess(std::wstring const& command, std::wstring const& workingDirectory, int& exitCode);
std::wstring toWide(std::string_view text);
std::string fromWide(std::wstring_view text);

} // namespace platform

} // namespace clow
