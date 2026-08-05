#include "clow/checksum.hpp"
#include "clow/logger.hpp"
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <windows.h>
#include <wincrypt.h>

namespace clow {
namespace checksum {

std::string sha256File(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        Logger::error("Unable to open file for checksum: " + path.string());
        return {};
    }

    HCRYPTPROV provider = 0;
    if (!CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        Logger::error("Failed to acquire crypto context");
        return {};
    }

    HCRYPTHASH hash = 0;
    if (!CryptCreateHash(provider, CALG_SHA_256, 0, 0, &hash)) {
        CryptReleaseContext(provider, 0);
        Logger::error("Failed to create hash object");
        return {};
    }

    std::array<char, 8192> buffer;
    while (input.good()) {
        input.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        std::streamsize bytesRead = input.gcount();
        if (bytesRead > 0 && !CryptHashData(hash, reinterpret_cast<BYTE*>(buffer.data()), static_cast<DWORD>(bytesRead), 0)) {
            CryptDestroyHash(hash);
            CryptReleaseContext(provider, 0);
            Logger::error("Failed to hash file data");
            return {};
        }
    }

    DWORD length = 0;
    if (!CryptGetHashParam(hash, HP_HASHVAL, nullptr, &length, 0)) {
        CryptDestroyHash(hash);
        CryptReleaseContext(provider, 0);
        Logger::error("Failed to query hash length");
        return {};
    }

    std::vector<BYTE> digest(length);
    if (!CryptGetHashParam(hash, HP_HASHVAL, digest.data(), &length, 0)) {
        CryptDestroyHash(hash);
        CryptReleaseContext(provider, 0);
        Logger::error("Failed to retrieve hash value");
        return {};
    }

    std::ostringstream output;
    output << std::hex;
    for (BYTE byte : digest) {
        output.width(2);
        output.fill('0');
        output << static_cast<int>(byte);
    }

    CryptDestroyHash(hash);
    CryptReleaseContext(provider, 0);
    return output.str();
}

bool verifyChecksum(const std::filesystem::path& path, const std::string& expectedChecksum) {
    const auto actualChecksum = sha256File(path);
    if (actualChecksum.empty()) {
        return false;
    }
    if (expectedChecksum.rfind("sha256:", 0) == 0) {
        return actualChecksum == expectedChecksum.substr(7);
    }
    return actualChecksum == expectedChecksum;
}

} // namespace checksum
} // namespace clow
