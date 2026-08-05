# Contributing to CLOW

Thank you for your interest in contributing to CLOW. This project is built with modern C++ and a strong focus on modular architecture, reliability, and cross-platform Windows build automation.

## Contribution Process

1. Fork the repository.
2. Create a feature branch from `main`.
3. Add tests for any new behavior.
4. Open a pull request with a clear summary of changes.

## Coding Guidelines

- Use C++20 and the STL.
- Prefer RAII and strong typing.
- Avoid global mutable state.
- Keep classes small and single responsibility.
- Write clear and concise documentation comments.

## Testing

Run tests with CMake and CTest:

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
ctest --output-on-failure
```
