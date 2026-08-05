#include "clow/storage.hpp"
#include "clow/platform.hpp"
#include <filesystem>

namespace clow {

std::filesystem::path Storage::distrosDirectory() noexcept {
    return std::filesystem::current_path() / ".." / ".." / "distros";
}

std::filesystem::path Storage::runtimeDirectory() noexcept {
    auto path = platform::appDataPath();
    path /= "instances";
    return path;
}

std::filesystem::path Storage::globalConfigPath() noexcept {
    auto path = platform::appDataPath();
    path /= "global_config.json";
    return path;
}

std::filesystem::path Storage::instancePath(const std::string& key) noexcept {
    return runtimeDirectory() / key;
}

bool Storage::ensureStorageDirectories() noexcept {
    return platform::ensureDirectory(runtimeDirectory());
}

std::vector<std::string> Storage::listInstalledDistributions() noexcept {
    std::vector<std::string> result;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(runtimeDirectory(), ec)) {
        if (!ec && entry.is_directory()) {
            result.push_back(entry.path().filename().string());
        }
    }
    return result;
}

} // namespace clow
