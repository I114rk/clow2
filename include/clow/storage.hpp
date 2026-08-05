#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace clow {

/// Storage helpers for distribution metadata, runtime state, and configuration.
class Storage {
public:
    static std::filesystem::path distrosDirectory() noexcept;
    static std::filesystem::path runtimeDirectory() noexcept;
    static std::filesystem::path globalConfigPath() noexcept;
    static std::filesystem::path instancePath(const std::string& key) noexcept;
    static bool ensureStorageDirectories() noexcept;
    static std::vector<std::string> listInstalledDistributions() noexcept;
};

} // namespace clow
