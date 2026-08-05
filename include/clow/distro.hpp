#pragma once

#include "clow/json.hpp"
#include <filesystem>
#include <string>

namespace clow {

/// Manifest describing a supported distribution.
class DistroManifest {
public:
    DistroManifest() = default;
    DistroManifest(std::string name, std::string version, std::string rootfsUrl, std::string checksum, int defaultRam, int defaultCpu, int defaultDisk);

    static DistroManifest loadFromFile(const std::filesystem::path& path);

    std::string name() const noexcept;
    std::string version() const noexcept;
    std::string rootfsUrl() const noexcept;
    std::string checksum() const noexcept;
    int defaultRam() const noexcept;
    int defaultCpu() const noexcept;
    int defaultDisk() const noexcept;
    std::string key() const noexcept;

private:
    std::string name_;
    std::string version_;
    std::string rootfsUrl_;
    std::string checksum_;
    int defaultRam_ = 4096;
    int defaultCpu_ = 4;
    int defaultDisk_ = 20;
};

} // namespace clow
