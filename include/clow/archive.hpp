#pragma once

#include <filesystem>
#include <string>

namespace clow {

namespace archive {

/// Creates a compressed archive from a source directory.
bool createArchive(const std::filesystem::path& sourceDirectory, const std::filesystem::path& archivePath);

/// Extracts a compressed archive to a target directory.
bool extractArchive(const std::filesystem::path& archivePath, const std::filesystem::path& targetDirectory);

} // namespace archive

} // namespace clow
