#pragma once

#include "clow/json.hpp"
#include <string>

namespace clow {

/// Distribution configuration options persisted in the registry.
class Configuration {
public:
    Configuration() = default;

    static Configuration defaultConfiguration() noexcept;
    static Configuration fromJson(const JsonValue& value);
    JsonValue toJson() const;

    std::string installationDrive() const noexcept;
    void setInstallationDrive(std::string value) noexcept;

    int ramMb() const noexcept;
    void setRamMb(int value) noexcept;

    int cpuCount() const noexcept;
    void setCpuCount(int value) noexcept;

    int diskSizeGb() const noexcept;
    void setDiskSizeGb(int value) noexcept;

    std::string hostname() const noexcept;
    void setHostname(std::string value) noexcept;

    std::string username() const noexcept;
    void setUsername(std::string value) noexcept;

    std::string password() const noexcept;
    void setPassword(std::string value) noexcept;

    std::string sharedFolders() const noexcept;
    void setSharedFolders(std::string value) noexcept;

    std::string network() const noexcept;
    void setNetwork(std::string value) noexcept;

private:
    std::string installationDrive_ = "C:";
    int ramMb_ = 4096;
    int cpuCount_ = 4;
    int diskSizeGb_ = 20;
    std::string hostname_ = "clow";
    std::string username_ = "clowuser";
    std::string password_ = "clowpass";
    std::string sharedFolders_ = "";
    std::string network_ = "nat";
};

} // namespace clow
