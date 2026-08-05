# CLOW Architecture

CLOW is designed as a modular Windows-native distribution manager with clean separation of responsibilities.

## Modules

- `cli`: Command line user interface and argument parsing.
- `core`: Runtime orchestration, distribution registry, configuration, and platform utilities.
- `installer`: Download, verify, extract, and register distribution root filesystems.
- `storage`: Distribution metadata and state persistence.
- `platform`: Windows filesystem, process, and networking helpers.
- `json`: Manifest parsing and serialization.
- `logging`: Console output with progress, warnings, and error reporting.

## Directory Structure

- `apps/clow`: CLI entry point.
- `core`: Core implementation and public interfaces.
- `include/clow`: API headers.
- `distros`: Distribution JSON manifests.
- `tests`: Unit tests for modules.
