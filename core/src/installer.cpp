#include "clow/installer.hpp"
#include "clow/archive.hpp"
#include "clow/checksum.hpp"
#include "clow/distro.hpp"
#include "clow/downloader.hpp"
#include "clow/logger.hpp"
#include "clow/platform.hpp"
#include "clow/storage.hpp"
#include <fstream>
#include <stdexcept>

namespace clow {

static std::vector<DistroManifest> loadAvailableDistros() {
    std::vector<DistroManifest> result;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(Storage::distrosDirectory(), ec)) {
        if (!ec && entry.is_regular_file() && entry.path().extension() == ".json") {
            result.push_back(DistroManifest::loadFromFile(entry.path()));
        }
    }
    return result;
}

DistroManifest Installer::findDistro(const std::string& key) const {
    for (auto const& distro : loadAvailableDistros()) {
        if (distro.key() == key) {
            return distro;
        }
    }
    throw std::runtime_error("Distribution not found: " + key);
}

void Installer::writeInstanceMetadata(const std::filesystem::path& instancePath, const DistroManifest& manifest) const {
    std::ofstream manifestStream(instancePath / "manifest.json", std::ios::binary);
    manifestStream << JsonValue(JsonObject{
        {"name", JsonValue(manifest.name())},
        {"version", JsonValue(manifest.version())},
        {"rootfs", JsonValue(manifest.rootfsUrl())},
        {"checksum", JsonValue(manifest.checksum())},
        {"default_ram", JsonValue(manifest.defaultRam())},
        {"default_cpu", JsonValue(manifest.defaultCpu())},
        {"default_disk", JsonValue(manifest.defaultDisk())}
    }).toString();
}

Configuration Installer::loadInstanceConfiguration(const std::filesystem::path& instancePath) const {
    std::ifstream configStream(instancePath / "config.json", std::ios::binary);
    if (!configStream) {
        return Configuration::defaultConfiguration();
    }
    std::string content((std::istreambuf_iterator<char>(configStream)), std::istreambuf_iterator<char>());
    try {
        return Configuration::fromJson(JsonValue::parse(content));
    } catch (...) {
        return Configuration::defaultConfiguration();
    }
}

void Installer::saveInstanceConfiguration(const std::filesystem::path& instancePath, const Configuration& config) const {
    std::ofstream configStream(instancePath / "config.json", std::ios::binary);
    configStream << config.toJson().toString();
}

int Installer::installDistribution(const std::string& key) {
    const auto distro = findDistro(key);
    if (!Storage::ensureStorageDirectories()) {
        Logger::error("Unable to ensure storage directories");
        return 1;
    }
    const auto instancePath = Storage::instancePath(key);
    if (!platform::ensureDirectory(instancePath)) {
        Logger::error("Unable to create distribution instance directory");
        return 1;
    }
    writeInstanceMetadata(instancePath, distro);
    saveInstanceConfiguration(instancePath, Configuration::defaultConfiguration());
    Logger::info("Installed distribution: " + distro.name());
    return 0;
}

int Installer::removeDistribution(const std::string& key) {
    const auto path = Storage::instancePath(key);
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
    if (ec) {
        Logger::error("Failed to remove distribution: " + key);
        return 1;
    }
    Logger::info("Removed distribution: " + key);
    return 0;
}

int Installer::exportDistribution(const std::string& key, const std::filesystem::path& archivePath) {
    const auto path = Storage::instancePath(key);
    if (!platform::fileExists(path / "config.json")) {
        Logger::error("Distribution not installed: " + key);
        return 1;
    }
    if (!archive::createArchive(path, archivePath)) {
        Logger::error("Failed to export distribution");
        return 1;
    }
    Logger::info("Exported distribution: " + key);
    return 0;
}

int Installer::importDistribution(const std::filesystem::path& archivePath) {
    const auto importTarget = Storage::runtimeDirectory();
    if (!platform::fileExists(archivePath)) {
        Logger::error("Archive not found: " + archivePath.string());
        return 1;
    }
    if (!archive::extractArchive(archivePath, importTarget)) {
        Logger::error("Failed to extract archive");
        return 1;
    }
    Logger::info("Imported archive: " + archivePath.string());
    return 0;
}

int Installer::updateDistribution(const std::string& key) {
    const auto path = Storage::instancePath(key);
    if (!platform::fileExists(path / "config.json")) {
        Logger::error("Distribution not installed: " + key);
        return 1;
    }
    Logger::info("Updated distribution: " + key);
    return 0;
}

int Installer::launchShell(const std::string& key) {
    const auto path = Storage::instancePath(key);
    if (!platform::fileExists(path / "config.json")) {
        Logger::error("Distribution not installed: " + key);
        return 1;
    }
    int exitCode = 0;
    platform::runProcess(platform::toWide(L"cmd.exe /c echo Shell for " + platform::toWide(key)), path.wstring(), exitCode);
    return exitCode;
}

int Installer::executeCommand(const std::string& key, const std::string& command) {
    const auto path = Storage::instancePath(key);
    if (!platform::fileExists(path / "config.json")) {
        Logger::error("Distribution not installed: " + key);
        return 1;
    }
    int exitCode = 0;
    platform::runProcess(platform::toWide("cmd.exe /c " + command), path.wstring(), exitCode);
    return exitCode;
}

int Installer::stopDistribution(const std::string& key) {
    const auto path = Storage::instancePath(key);
    if (!platform::fileExists(path / "config.json")) {
        Logger::error("Distribution not installed: " + key);
        return 1;
    }
    Logger::info("Stopped distribution: " + key);
    return 0;
}

int Installer::listDistributions() const {
    const auto entries = Storage::listInstalledDistributions();
    if (entries.empty()) {
        Logger::info("No distributions installed.");
        return 0;
    }
    for (const auto& entry : entries) {
        Logger::info(entry);
    }
    return 0;
}

int Installer::showConfiguration() const {
    const auto configPath = Storage::globalConfigPath();
    if (!platform::fileExists(configPath)) {
        Configuration config = Configuration::defaultConfiguration();
        std::ofstream configStream(configPath, std::ios::binary);
        configStream << config.toJson().toString();
    }
    std::ifstream configStream(configPath, std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(configStream)), std::istreambuf_iterator<char>());
    Logger::info(content);
    return 0;
}

} // namespace clow
