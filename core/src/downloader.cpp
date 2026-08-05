#include "clow/downloader.hpp"
#include "clow/logger.hpp"
#include "clow/platform.hpp"
#include <fstream>
#include <vector>
#include <windows.h>
#include <winhttp.h>

namespace clow {
namespace downloader {

bool downloadFile(const std::wstring& url, const std::filesystem::path& destination) {
    Logger::progress("Downloading " + platform::fromWide(url));
    URL_COMPONENTS components{};
    components.dwStructSize = sizeof(components);

    const auto internet = WinHttpOpen(L"CLOW Downloader/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!internet) {
        Logger::error("WinHTTP initialization failed");
        return false;
    }

    const auto cleanupHandle = [&](HINTERNET handle) {
        if (handle) {
            WinHttpCloseHandle(handle);
        }
    };

    if (!WinHttpCrackUrl(url.c_str(), static_cast<int>(url.size()), 0, &components)) {
        Logger::error("Failed to parse URL");
        cleanupHandle(internet);
        return false;
    }

    const auto session = WinHttpConnect(internet, components.lpszHostName, components.nPort, 0);
    if (!session) {
        Logger::error("Failed to connect to host");
        cleanupHandle(internet);
        return false;
    }

    const auto request = WinHttpOpenRequest(session, L"GET", components.lpszUrlPath, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, components.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!request) {
        Logger::error("Failed to open HTTP request");
        cleanupHandle(session);
        cleanupHandle(internet);
        return false;
    }

    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request, nullptr)) {
        Logger::error("Failed to complete HTTP request");
        cleanupHandle(request);
        cleanupHandle(session);
        cleanupHandle(internet);
        return false;
    }

    std::ofstream file(destination, std::ios::binary);
    if (!file) {
        Logger::error("Failed to open download destination");
        cleanupHandle(request);
        cleanupHandle(session);
        cleanupHandle(internet);
        return false;
    }

    while (true) {
        DWORD bytesAvailable = 0;
        if (!WinHttpQueryDataAvailable(request, &bytesAvailable) || bytesAvailable == 0) {
            break;
        }
        std::vector<char> buffer(bytesAvailable);
        DWORD bytesRead = 0;
        if (!WinHttpReadData(request, buffer.data(), bytesAvailable, &bytesRead) || bytesRead == 0) {
            break;
        }
        file.write(buffer.data(), static_cast<std::streamsize>(bytesRead));
    }

    cleanupHandle(request);
    cleanupHandle(session);
    cleanupHandle(internet);
    Logger::info("Downloaded to " + destination.string());
    return true;
}

} // namespace downloader
} // namespace clow
