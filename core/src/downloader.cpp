#include "clow/downloader.hpp"
#include "clow/logger.hpp"
#include "clow/platform.hpp"
#include <fstream>
#include <windows.h>

namespace clow {
namespace downloader {

bool downloadFile(const std::wstring& url, const std::filesystem::path& destination) {
    Logger::progress("Downloading " + platform::fromWide(url));
    URL_COMPONENTS components{};
    components.dwStructSize = sizeof(components);

    HINTERNET internet = WinHttpOpen(L"CLOW Downloader/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!internet) {
        Logger::error("WinHTTP initialization failed");
        return false;
    }

    bool success = false;
    HINTERNET session = nullptr;
    HINTERNET connect = nullptr;
    HINTERNET request = nullptr;

    // Very basic URL parsing using WinHTTP.
    wchar_t hostName[256] = {};
    wchar_t urlPath[2048] = {};
    components.lpszHostName = hostName;
    components.dwHostNameLength = _countof(hostName);
    components.lpszUrlPath = urlPath;
    components.dwUrlPathLength = _countof(urlPath);
    if (!WinHttpCrackUrl(url.c_str(), static_cast<int>(url.size()), 0, &components)) {
        Logger::error("Failed to parse URL");
        goto cleanup;
    }

    session = WinHttpConnect(internet, hostName, components.nPort, 0);
    if (!session) {
        Logger::error("Failed to connect to host");
        goto cleanup;
    }

    request = WinHttpOpenRequest(session, L"GET", components.lpszUrlPath, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, components.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!request) {
        Logger::error("Failed to open HTTP request");
        goto cleanup;
    }

    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request, nullptr)) {
        Logger::error("Failed to complete HTTP request");
        goto cleanup;
    }

    std::ofstream file(destination, std::ios::binary);
    if (!file) {
        Logger::error("Failed to open download destination");
        goto cleanup;
    }

    DWORD bytesAvailable = 0;
    while (WinHttpQueryDataAvailable(request, &bytesAvailable) && bytesAvailable > 0) {
        std::vector<char> buffer(bytesAvailable);
        DWORD bytesRead = 0;
        if (!WinHttpReadData(request, buffer.data(), bytesAvailable, &bytesRead) || bytesRead == 0) {
            break;
        }
        file.write(buffer.data(), bytesRead);
    }

    success = true;
    Logger::info("Downloaded to " + destination.string());

cleanup:
    if (request) WinHttpCloseHandle(request);
    if (connect) WinHttpCloseHandle(connect);
    if (session) WinHttpCloseHandle(session);
    if (internet) WinHttpCloseHandle(internet);
    return success;
}

} // namespace downloader
} // namespace clow
