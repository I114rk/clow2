#include "clow/platform.hpp"
#include <Windows.h>
#include <shlobj.h>
#include <stdexcept>
#include <system_error>

namespace clow {

namespace platform {

std::filesystem::path appDataPath() {
    wchar_t path[MAX_PATH] = {0};
    if (FAILED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, path))) {
        throw std::runtime_error("Unable to resolve AppData path");
    }
    return std::filesystem::path(path) / L"CLOW";
}

bool ensureDirectory(const std::filesystem::path& directory) noexcept {
    std::error_code ec;
    std::filesystem::create_directories(directory, ec);
    return !ec;
}

bool fileExists(const std::filesystem::path& filePath) noexcept {
    std::error_code ec;
    return std::filesystem::exists(filePath, ec);
}

std::wstring toWide(std::string_view text) {
    if (text.empty()) {
        return {};
    }
    int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size);
    return result;
}

std::string fromWide(std::wstring_view text) {
    if (text.empty()) {
        return {};
    }
    int size = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
    std::string result(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size, nullptr, nullptr);
    return result;
}

std::string runProcess(std::wstring const& command, std::wstring const& workingDirectory, int& exitCode) {
    STARTUPINFOW startupInfo{};
    PROCESS_INFORMATION processInfo{};
    startupInfo.cb = sizeof(startupInfo);

    std::wstring commandLine = command;
    if (!CreateProcessW(nullptr, commandLine.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, workingDirectory.empty() ? nullptr : workingDirectory.c_str(), &startupInfo, &processInfo)) {
        exitCode = -1;
        return "";
    }

    WaitForSingleObject(processInfo.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(processInfo.hProcess, &code);
    exitCode = static_cast<int>(code);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return "";
}

} // namespace platform

} // namespace clow
