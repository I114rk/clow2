# CLOW

CLOW (C++ Linux On Windows) is an open-source, modern C++ alternative to WSL.
It provides CLI-based Linux distribution lifecycle management, container-style roots, and an extensible Windows-native runtime.

## Features

- Install Arch, Ubuntu, and Debian distributions
- Shell entry and command execution with `clow shell` and `clow exec`
- Stop, remove, list, export, import, and update distributions
- Configurable RAM, CPU count, disk size, hostname, username, and shared folders
- Modern C++20 architecture with modular components
- Build using CMake and MinGW-w64 on Windows

## Quick Start

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
.
apps\clow\clow.exe install arch
clow list
clow shell arch
``` 

## Installation

1. Install MinGW-w64 and CMake.
2. Clone the repository.
3. Run:

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

## Usage

```text
clow install <distro>
clow shell <distro>
clow exec <distro> <command>
clow stop <distro>
clow remove <distro>
clow list
clow export <distro> <archive>
clow import <archive>
clow config
clow update <distro>
```

## Architecture

- `apps/clow`: Main CLI executable.
- `core`: Core runtime, installer, storage, and command orchestration.
- `include/clow`: Public headers for core modules.
- `distros`: Distribution manifest JSON files.
- `tests`: Unit and integration tests.

## Contributing

See `CONTRIBUTING.md`.

## License

MIT License. See `LICENSE`.
