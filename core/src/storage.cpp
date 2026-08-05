#include "clow/storage.hpp"
#include "clow/platform.hpp"
#include <filesystem>

namespace clow {

namespace {

bool hasManifests(const std::filesystem::path& directory) noexcept {
    std::error_code ec;
    return std::filesystem::is_directory(directory, ec);
}

std::filesystem::path searchUpwards(std::filesystem::path start) noexcept {
    for (int depth = 0; depth < 6 && !start.empty(); ++depth) {
        const auto candidate = start / "distros";
        if (hasManifests(candidate)) {
            return candidate;
        }
        const auto parent = start.parent_path();
        if (parent == start) {
            break;
        }
        start = parent;
    }
    return {};
}

} // namespace

std::filesystem::path Storage::distrosDirectory() noexcept {
    std::error_code ec;

    try {
        const auto fromExecutable = searchUpwards(platform::executablePath().parent_path());
        if (!fromExecutable.empty()) {
            return fromExecutable;
        }
    } catch (...) {
        // Fall through to the working-directory search below.
    }

    const auto fromWorkingDirectory = searchUpwards(std::filesystem::current_path(ec));
    if (!ec && !fromWorkingDirectory.empty()) {
        return fromWorkingDirectory;
    }

    return std::filesystem::path("distros");
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
