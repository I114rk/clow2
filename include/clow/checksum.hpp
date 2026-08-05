#pragma once

#include <filesystem>
#include <string>

namespace clow {

namespace checksum {

/// Computes the SHA256 hash of a local file.
std::string sha256File(const std::filesystem::path& path);

/// Compares the computed checksum against a value with optional prefix.
bool verifyChecksum(const std::filesystem::path& path, const std::string& expectedChecksum);

} // namespace checksum

} // namespace clow
