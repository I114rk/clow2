#pragma once

#include "clow/distro.hpp"
#include "clow/configuration.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace clow {

/// Manages distribution lifecycle operations.
class Installer {
public:
    Installer() = default;

    int installDistribution(const std::string& key);
    int removeDistribution(const std::string& key);
    int exportDistribution(const std::string& key, const std::filesystem::path& archivePath);
    int importDistribution(const std::filesystem::path& archivePath);
    int updateDistribution(const std::string& key);
    int launchShell(const std::string& key);
    int executeCommand(const std::string& key, const std::string& command);
    int stopDistribution(const std::string& key);
    int listDistributions() const;
    int showConfiguration() const;

private:
    DistroManifest findDistro(const std::string& key) const;
    void writeInstanceMetadata(const std::filesystem::path& instancePath, const DistroManifest& manifest) const;
    Configuration loadInstanceConfiguration(const std::filesystem::path& instancePath) const;
    void saveInstanceConfiguration(const std::filesystem::path& instancePath, const Configuration& config) const;
};

} // namespace clow
