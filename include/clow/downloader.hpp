#pragma once

#include <filesystem>
#include <string>

namespace clow {

namespace downloader {

/// Downloads a remote file from a URL to a local destination path.
/// Returns true when the download completes successfully.
bool downloadFile(const std::wstring& url, const std::filesystem::path& destination);

} // namespace downloader

} // namespace clow
