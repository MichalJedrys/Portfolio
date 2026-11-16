# PlayRuntime

PlayRuntime is a planned C++20 interaction engine that turns sensor events into scripted light and sound commands. A desktop simulator will let users create, record and replay interactions without physical hardware.

## Status

The project currently contains the design documents and a C++20 build scaffold: a static core library, a CLI exposing build information, and smoke tests. Interaction execution, scripting and replay are not implemented, and no runtime performance is measured. Development takes place on `develop-play-runtime`.

## Example interaction

A button starts a three-second quest. Detecting blue and then motion completes it with a green light and success sound. A timeout produces a red light and failure sound. Both outcomes enter a one-second cooldown before another quest can start.

The engine will own states, timers and rule execution. A small domain-specific language will describe behaviours without recompiling C++. Host adapters will supply sensor events and consume commands. Recording and replay will reproduce the logical interaction for debugging; they will not promise identical physical audio timing.

## First release

- A reusable deterministic core with explicit resource limits.
- A small validated scripting language and headless CLI.
- Versioned session recording and exact logical trace replay.
- An optional desktop simulator with virtual sensors and sound cues.
- Automated correctness checks and reproducible performance measurements.

Physical sensors, wireless protocols and firmware deployment are later extensions. This is an independent portfolio project, not a LEGO product or an implementation of a LEGO protocol.

## Design documents

- [Requirements and execution contract](docs/requirements.md): scope, semantics, resource limits, example behaviour and acceptance traces.
- [Architecture](docs/architecture.md): components, ownership and data flow.
- [ADR 0001](docs/adr/0001-deterministic-core.md): the decision to use a single-owner deterministic core.

## Build and smoke tests

Prerequisites: CMake 3.24 or newer, Ninja, and a C++20 compiler available in your terminal. The test dependency also configures a C compiler. On Windows with MSVC, use a Visual Studio developer terminal so the compiler and SDK are available. With LLVM-MinGW, keep its bin directory on PATH while building and running, including for its runtime DLLs.

From `PlayRuntime/`:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

For Release, replace `debug` with `release`. To select a compiler on a fresh build directory, pass e.g. `-DCMAKE_CXX_COMPILER=clang++` to the configure command. Use a separate build directory when changing toolchains; do not reuse a compiler-specific cache.

The presets write to `../build/play-runtime/debug` and `../build/play-runtime/release`, which are ignored by Git. No generated file belongs in a tracked `output/` directory. The build is independent of the other portfolio projects and does not use their prebuilt GoogleTest archives.

Run the smoke CLI from `PlayRuntime/` on Windows:

```powershell
..\build\play-runtime\debug\playruntime.exe --version
..\build\play-runtime\debug\playruntime.exe --help
```

On Linux, use `../build/play-runtime/debug/playruntime`. No arguments displays help; unsupported arguments return exit code 2. The version identifies the scaffold, not a completed v0.1 release.

### Dependencies and offline builds

Tests default to enabled. On first configuration, CMake downloads GoogleTest v1.17.0 from its official source archive and verifies the SHA-256 pinned in `tests/CMakeLists.txt`. It compiles GoogleTest for the selected toolchain. Network access is needed for this initial download, not to run tests. SDL is not a dependency of this scaffold.

For an offline test build, supply an already extracted copy of the same GoogleTest source with `-DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=/path/to/googletest`. This override bypasses the archive download/hash check, so the caller must verify that source. Do not point it at the older portfolio GoogleTest copies.

To build only the core and CLI without downloading any test dependency, from `PlayRuntime/`:

```sh
cmake -S . -B ../build/play-runtime/no-tests -G Ninja -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build ../build/play-runtime/no-tests
```

### Validation recorded for this scaffold

Windows x64 using LLVM-MinGW 20261104 (Clang 23.1.5), CMake 4.5.0, Ninja 1.13.2 and GoogleTest 1.17.0:

- Debug and Release builds and CTest smoke suites.
- CTest discovery of the linked core metadata test and CLI integration test.
- A temporary failing GoogleTest case produced a failing CTest exit code; the probe was removed and the passing suite rebuilt.
- A separate `BUILD_TESTING=OFF` build produced the CLI without fetching GoogleTest.

These checks validate build integration and CLI plumbing only, not engine behaviour. Linux, MSVC and the minimum CMake version have not been exercised locally. The next milestone adds the cross-platform CI matrix.
