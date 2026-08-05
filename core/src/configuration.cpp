#include "clow/configuration.hpp"

namespace clow {

Configuration Configuration::defaultConfiguration() noexcept {
    return Configuration();
}

Configuration Configuration::fromJson(const JsonValue& value) {
    Configuration configuration;
    if (!value.isObject()) {
        return configuration;
    }
    const auto& object = value.asObject();
    auto atString = [&](std::string_view key, std::string& target) {
        auto it = object.find(std::string(key));
        if (it != object.end() && it->second.isString()) {
            target = it->second.asString();
        }
    };
    auto atNumber = [&](std::string_view key, int& target) {
        auto it = object.find(std::string(key));
        if (it != object.end() && it->second.isNumber()) {
            target = static_cast<int>(it->second.asNumber());
        }
    };

    atString("installation_drive", configuration.installationDrive_);
    atNumber("ram_mb", configuration.ramMb_);
    atNumber("cpu_count", configuration.cpuCount_);
    atNumber("disk_size_gb", configuration.diskSizeGb_);
    atString("hostname", configuration.hostname_);
    atString("username", configuration.username_);
    atString("password", configuration.password_);
    atString("shared_folders", configuration.sharedFolders_);
    atString("network", configuration.network_);
    return configuration;
}

JsonValue Configuration::toJson() const {
    JsonObject object;
    object["installation_drive"] = JsonValue(installationDrive_);
    object["ram_mb"] = JsonValue(ramMb_);
    object["cpu_count"] = JsonValue(cpuCount_);
    object["disk_size_gb"] = JsonValue(diskSizeGb_);
    object["hostname"] = JsonValue(hostname_);
    object["username"] = JsonValue(username_);
    object["password"] = JsonValue(password_);
    object["shared_folders"] = JsonValue(sharedFolders_);
    object["network"] = JsonValue(network_);
    return JsonValue(std::move(object));
}

std::string Configuration::installationDrive() const noexcept { return installationDrive_; }
void Configuration::setInstallationDrive(std::string value) noexcept { installationDrive_ = std::move(value); }
int Configuration::ramMb() const noexcept { return ramMb_; }
void Configuration::setRamMb(int value) noexcept { ramMb_ = value; }
int Configuration::cpuCount() const noexcept { return cpuCount_; }
void Configuration::setCpuCount(int value) noexcept { cpuCount_ = value; }
int Configuration::diskSizeGb() const noexcept { return diskSizeGb_; }
void Configuration::setDiskSizeGb(int value) noexcept { diskSizeGb_ = value; }
std::string Configuration::hostname() const noexcept { return hostname_; }
void Configuration::setHostname(std::string value) noexcept { hostname_ = std::move(value); }
std::string Configuration::username() const noexcept { return username_; }
void Configuration::setUsername(std::string value) noexcept { username_ = std::move(value); }
std::string Configuration::password() const noexcept { return password_; }
void Configuration::setPassword(std::string value) noexcept { password_ = std::move(value); }
std::string Configuration::sharedFolders() const noexcept { return sharedFolders_; }
void Configuration::setSharedFolders(std::string value) noexcept { sharedFolders_ = std::move(value); }
std::string Configuration::network() const noexcept { return network_; }
void Configuration::setNetwork(std::string value) noexcept { network_ = std::move(value); }

} // namespace clow
