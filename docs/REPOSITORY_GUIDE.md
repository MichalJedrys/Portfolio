# Repository guide

[Back to portfolio](../README.md)

## Existing layout

Active projects are kept at the repository root. Snake Game and the Python vending-machine project are stored under `archive/`.

| Directory | Purpose |
| --- | --- |
| `Library Management System/` | C++17 book catalogue, SQLite, and database tests |
| `Srt Parser Project/` | C++17 subtitle parser, samples, and tests |
| `archive/Snake Game/` | C++11 Windows console game |
| `Vending Machine/` | C++17 console simulation |
| `archive/Vending Machine (Python)/` | Python variant of the simulation |
| `docs/` | Repository navigation and development notes |

## C++ build starting points

Each C++ project has its own CMake configuration. From the repository root:

```powershell
cmake -S "Library Management System" -B build/library
cmake --build build/library --config Release
ctest --test-dir build/library -C Release --output-on-failure
```

Use the corresponding source directory and a distinct build directory for each project. Library, parser, and C++ vending machine require C++17; Snake requires C++11 and Windows. Only library and parser have project-owned GoogleTest suites.

These commands are starting points, not a verified build recipe. Library and parser configurations import GoogleTest archives from fixed `lib/libgtest*.a` and `lib/libgmock*.a` paths, which may not match the generator or compiler, especially MSVC. Reproducible build support needs verification. Existing CMake targets also write executables into tracked project `output/` directories even when configured out of tree.

Run database tests and demos in disposable working directories. The library creates database files; the parser uses fixed input/output filenames. Preserve committed samples when experimenting. See each project README for runtime details.
