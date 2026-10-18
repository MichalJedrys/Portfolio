# Repository guide

[Back to portfolio](../README.md)

## Existing layout

Original project directories and public paths are retained. This guide adds navigation without relocating source, samples, dependencies, or binaries.

| Directory | Purpose |
| --- | --- |
| `Library Management System/` | C++17 book catalogue, SQLite, and database tests |
| `Srt Parser Project/` | C++17 subtitle parser, samples, and tests |
| `Snake Game/` | C++11 Windows console game |
| `Vending Machine/` | C++17 console simulation |
| `Vending Machine (Python)/` | Python variant of the simulation |
| `docs/` | Repository navigation and development notes |
| `projects/` | Current project index and new showcase implementations |
| `archive/` | Navigation to earlier exercises at their original paths |

Library, parser, and C++ vending-machine projects keep library code in `source/` and executable entry points in `output/src/`. Library and parser tests are under `test/`, with bundled dependencies under `3rd_party/`. Snake uses `src/` and `include/`; Python uses `source/`.

Despite its name, `output/` contains tracked source and sample data as well as executables. Do not delete or blanket-ignore it as generated build output.

## C++ build starting points

The new [SRT Parser](../projects/srt-parser/README.md) and [scheduling simulator](../projects/scheduling-simulator/README.md) have independent CMake 3.20+ builds and dependency-free tests. Their README commands build into `build/` without modifying the original projects. The [C++ workflow](../.github/workflows/portfolio-cpp.yml) builds and tests both on Linux and Windows.

The notes below describe the original projects, whose build configuration remains unchanged.

Each C++ project has its own CMake configuration. From the repository root:

```powershell
cmake -S "Library Management System" -B build/library
cmake --build build/library --config Release
ctest --test-dir build/library -C Release --output-on-failure
```

Use the corresponding source directory and a distinct build directory for each project. Library, parser, and C++ vending machine require C++17; Snake requires C++11 and Windows. Only library and parser have project-owned GoogleTest suites.

These commands are starting points, not a verified build recipe. Library and parser configurations import GoogleTest archives from fixed `lib/libgtest*.a` and `lib/libgmock*.a` paths, which may not match the generator or compiler, especially MSVC. Reproducible build support needs verification. Existing CMake targets also write executables into tracked project `output/` directories even when configured out of tree.

Run database tests and demos in disposable working directories. The library creates database files; the parser uses fixed input/output filenames. Preserve committed samples when experimenting. See each project README for runtime details.

## Development and release

See the [refactor plan](REFACTOR_PLAN.md) for project classification and the staged transition to the new layout.

- `main` remains the default, recruiter-facing version.
- Prepare and review this refactor on `develop`.
- Keep every existing public file and directory path until an explicitly approved release; assess links before later relocation or removal.
- Use normal new commits with truthful messages. Do not backdate commits, rewrite existing history, or invent prior project origins.
- Before proposing a release, verify README links, build/test results, runtime examples, and feature claims. Obtain explicit approval before merging or changing the default branch, visibility, or publishing configuration.

## Initial documentation pass

This pass updates navigation and descriptions based on source inspection and adds no application features. Build and runtime verification remain open: CMake and a C++ compiler were not available on the command path used for this pass.
