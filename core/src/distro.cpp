#include "clow/distro.hpp"
#include <fstream>
#include <stdexcept>

namespace clow {

DistroManifest::DistroManifest(std::string name, std::string version, std::string rootfsUrl, std::string checksum, int defaultRam, int defaultCpu, int defaultDisk)
    : name_(std::move(name)), version_(std::move(version)), rootfsUrl_(std::move(rootfsUrl)), checksum_(std::move(checksum)), defaultRam_(defaultRam), defaultCpu_(defaultCpu), defaultDisk_(defaultDisk) {}

DistroManifest DistroManifest::loadFromFile(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open distro manifest: " + path.string());
    }
    std::string content((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    JsonValue manifest = JsonValue::parse(content);
    const auto& object = manifest.asObject();

    auto getString = [&](const std::string& key) -> std::string {
        auto it = object.find(key);
        if (it == object.end() || !it->second.isString()) {
            throw std::runtime_error("Invalid distro manifest property: " + key);
        }
        return it->second.asString();
    };

    auto getInt = [&](const std::string& key) -> int {
        auto it = object.find(key);
        if (it == object.end() || !it->second.isNumber()) {
            throw std::runtime_error("Invalid distro manifest property: " + key);
        }
        return static_cast<int>(it->second.asNumber());
    };

    return DistroManifest(
        getString("name"),
        getString("version"),
        getString("rootfs"),
        getString("checksum"),
        getInt("default_ram"),
        getInt("default_cpu"),
        getInt("default_disk")
    );
}

std::string DistroManifest::name() const noexcept { return name_; }
std::string DistroManifest::version() const noexcept { return version_; }
std::string DistroManifest::rootfsUrl() const noexcept { return rootfsUrl_; }
std::string DistroManifest::checksum() const noexcept { return checksum_; }
int DistroManifest::defaultRam() const noexcept { return defaultRam_; }
int DistroManifest::defaultCpu() const noexcept { return defaultCpu_; }
int DistroManifest::defaultDisk() const noexcept { return defaultDisk_; }
std::string DistroManifest::key() const noexcept { return name_ == "Arch Linux" ? "arch" : name_ == "Ubuntu" ? "ubuntu" : name_ == "Debian" ? "debian" : name_; }

} // namespace clow
