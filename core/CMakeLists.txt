add_library(clow_core STATIC
    src/logger.cpp
    src/json.cpp
    src/platform.cpp
    src/distro.cpp
    src/configuration.cpp
    src/downloader.cpp
    src/checksum.cpp
    src/archive.cpp
    src/storage.cpp
    src/installer.cpp
)

target_include_directories(clow_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(clow_core PUBLIC cxx_std_20)

target_link_libraries(clow_core PRIVATE
    $<$<PLATFORM_ID:Windows>:winhttp;crypt32>
)
