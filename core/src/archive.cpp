#include "clow/archive.hpp"
#include "clow/logger.hpp"
#include <filesystem>
#include <fstream>
#include <vector>

namespace clow {
namespace archive {

namespace {

constexpr std::string_view archiveMagic = "CLOW_ARCHIVE_V1";

void writeString(std::ofstream& stream, const std::string& text) {
    uint32_t length = static_cast<uint32_t>(text.size());
    stream.write(reinterpret_cast<const char*>(&length), sizeof(length));
    stream.write(text.data(), length);
}

bool readString(std::ifstream& stream, std::string& text) {
    uint32_t length = 0;
    if (!stream.read(reinterpret_cast<char*>(&length), sizeof(length))) {
        return false;
    }
    text.resize(length);
    return static_cast<bool>(stream.read(text.data(), length));
}

bool writeEntry(std::ofstream& stream, const std::filesystem::path& root, const std::filesystem::path& path) {
    const auto relativePath = std::filesystem::relative(path, root).generic_string();
    writeString(stream, relativePath);
    const bool isDirectory = std::filesystem::is_directory(path);
    stream.write(reinterpret_cast<const char*>(&isDirectory), sizeof(isDirectory));
    if (isDirectory) {
        uint64_t size = 0;
        stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
        return true;
    }
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    file.seekg(0, std::ios::end);
    uint64_t size = static_cast<uint64_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
    std::vector<char> buffer(static_cast<size_t>(size));
    file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    stream.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    return true;
}

bool extractEntry(std::ifstream& stream, const std::filesystem::path& targetRoot) {
    std::string relativePath;
    if (!readString(stream, relativePath)) {
        return false;
    }
    bool isDirectory = false;
    if (!stream.read(reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory))) {
        return false;
    }
    uint64_t size = 0;
    if (!stream.read(reinterpret_cast<char*>(&size), sizeof(size))) {
        return false;
    }
    const auto targetPath = targetRoot / std::filesystem::path(relativePath);
    if (isDirectory) {
        std::error_code ec;
        std::filesystem::create_directories(targetPath, ec);
        return !ec;
    }
    std::error_code ec;
    std::filesystem::create_directories(targetPath.parent_path(), ec);
    if (ec) {
        return false;
    }
    std::ofstream out(targetPath, std::ios::binary);
    if (!out) {
        return false;
    }
    std::vector<char> buffer(static_cast<size_t>(size));
    if (!stream.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
        return false;
    }
    out.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    return true;
}

} // namespace

bool createArchive(const std::filesystem::path& sourceDirectory, const std::filesystem::path& archivePath) {
    if (!std::filesystem::exists(sourceDirectory)) {
        return false;
    }
    std::ofstream archiveFile(archivePath, std::ios::binary | std::ios::trunc);
    if (!archiveFile) {
        return false;
    }
    archiveFile.write(archiveMagic.data(), static_cast<std::streamsize>(archiveMagic.size()));
    archiveFile.put('\n');

    for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDirectory)) {
        if (!writeEntry(archiveFile, sourceDirectory, entry.path())) {
            return false;
        }
    }
    return true;
}

bool extractArchive(const std::filesystem::path& archivePath, const std::filesystem::path& targetDirectory) {
    std::ifstream archiveFile(archivePath, std::ios::binary);
    if (!archiveFile) {
        return false;
    }
    std::string magic;
    std::getline(archiveFile, magic);
    if (magic != archiveMagic) {
        return false;
    }
    std::error_code ec;
    std::filesystem::create_directories(targetDirectory, ec);
    if (ec) {
        return false;
    }
    while (archiveFile.peek() != EOF) {
        if (!extractEntry(archiveFile, targetDirectory)) {
            return false;
        }
    }
    return true;
}

} // namespace archive
} // namespace clow
