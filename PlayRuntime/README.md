# PlayRuntime

PlayRuntime is a C++20 interaction engine in development that turns sensor events into scripted light and sound commands. A desktop simulator will let users create, record and replay interactions without physical hardware.

## Status

The project currently implements typed button, colour and motion events, atomic batch submission, deterministic ordering and explicit logical time advancement. Programmatic rules map event kinds to light commands, with a runnable CLI demonstration and automated tests. Timers, state machines, scripting, session replay and the simulator are not implemented yet; runtime performance has not been measured. Development takes place on `develop-play-runtime`.

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

## Current runtime API and demo

`--demo` runs two button events with an unmatched motion event between them, using a programmatically configured button-to-cyan rule. It produces:

```text
0us event=0 light=cyan
1000000us event=2 light=cyan
```

See [the public API](include/playruntime/runtime.hpp), [implementation](src/core/runtime.cpp) and [tests](tests/runtime_test.cpp). A `Runtime` copies a list of `LightRule` entries. The first rule whose event kind matches emits one `SetLight`; colour rules currently match all valid sensor colours. This deliberately small API is not yet the planned script or state-machine representation.

`submit_batch` validates timestamps, strictly increasing session sequence IDs and typed payloads before inserting anything. Rejected batches leave the existing queue and sequence watermark unchanged. Events can arrive with out-of-order timestamps; execution sorts by timestamp, then sequence ID. Sequence IDs need not be contiguous and may begin at zero.

`advance_to` processes through an inclusive logical horizon and returns host-owned structured commands containing event time and source sequence. Advancing to the same horizon again is a no-op. Negative or decreasing horizons fail without consuming pending events. Once a horizon is closed, input at or before that time is rejected. There are no clocks, sleeps, I/O or callbacks in the core.

Errors are returned as `InputError` with a typed code and, for submission errors, the offending batch index. Invalid rule configuration throws `std::invalid_argument`. Allocation failure follows normal C++ exception behaviour; the queue replacement and output allocation occur before committing runtime mutations.

The current queue and returned output use dynamic vectors. Configurable capacity limits and per-advance work budgets belong to commit 7; no bounded-memory claim is made yet. `CommandRecord` is a minimal output record, not the future canonical trace with state/error records and ordinals. Reset, sound, timer tie ordering and serialized replay are also later steps. Repeating a run in tests verifies deterministic output, not session-file replay support.

### Validation for typed event dispatch (commit 4)

The Windows LLVM-MinGW toolchain listed below passes Debug and Release builds with strict warnings, clang-format 18 checks, 14 runtime tests, the linked-build test, and CLI integration checks. Tests cover chronological and same-time ordering across batches, atomic invalid-input rejection, empty input, first-rule precedence, closed horizons, integer limits and repeated/split advancement. Linux/MSVC and sanitizer validation for this change remain pending its first CI run after push.

## Build and tests

Prerequisites: CMake 3.24 or newer, Ninja, and a C++20 compiler available in your terminal. The test dependency also configures a C compiler. On Windows with MSVC, use a Visual Studio developer terminal so the compiler and SDK are available. With LLVM-MinGW, keep its bin directory on PATH while building and running, including for its runtime DLLs.

From `PlayRuntime/`:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

For Release, replace `debug` with `release`. To select a compiler on a fresh build directory, pass e.g. `-DCMAKE_CXX_COMPILER=clang++` to the configure command. Use a separate build directory when changing toolchains; do not reuse a compiler-specific cache.

The presets write to `../build/play-runtime/debug` and `../build/play-runtime/release`, which are ignored by Git. No generated file belongs in a tracked `output/` directory. The build is independent of the other portfolio projects and does not use their prebuilt GoogleTest archives.

Run the CLI from `PlayRuntime/` on Windows:

```powershell
..\build\play-runtime\debug\playruntime.exe --version
..\build\play-runtime\debug\playruntime.exe --help
..\build\play-runtime\debug\playruntime.exe --demo
```

On Linux, use `../build/play-runtime/debug/playruntime`. No arguments displays help; unsupported arguments return exit code 2. The version identifies the development build, not a completed v0.1 release.

### Dependencies and offline builds

Tests default to enabled. On first configuration, CMake downloads GoogleTest v1.17.0 from its official source archive and verifies the SHA-256 pinned in `tests/CMakeLists.txt`. It compiles GoogleTest for the selected toolchain. Network access is needed for this initial download, not to run tests. SDL is not a dependency of this scaffold.

For an offline test build, supply an already extracted copy of the same GoogleTest source with `-DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=/path/to/googletest`. This override bypasses the archive download/hash check, so the caller must verify that source. Do not point it at the older portfolio GoogleTest copies.

To build only the core and CLI without downloading any test dependency, from `PlayRuntime/`:

```sh
cmake -S . -B ../build/play-runtime/no-tests -G Ninja -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build ../build/play-runtime/no-tests
```

### Build-foundation validation (commit 2)

Windows x64 using LLVM-MinGW 20261104 (Clang 23.1.5), CMake 4.5.0, Ninja 1.13.2 and GoogleTest 1.17.0:

- Debug and Release builds and CTest smoke suites.
- CTest discovery of the linked core metadata test and CLI integration test.
- A temporary failing GoogleTest case produced a failing CTest exit code; the probe was removed and the passing suite rebuilt.
- A separate `BUILD_TESTING=OFF` build produced the CLI without fetching GoogleTest.

These checks validate build integration and CLI plumbing only, not engine behaviour. Linux, MSVC and the minimum CMake version have not been exercised locally. The CI configuration below adds the cross-platform matrix; its remote execution is still pending.

## Continuous integration and local quality checks

The [PlayRuntime workflow](../.github/workflows/play-runtime.yml) runs for relevant changes under `PlayRuntime/` or to the workflow itself. Pushes are limited to `main` and `develop-play-runtime`; pull requests targeting any branch are included. Manual dispatch is also declared (GitHub requires the workflow on the default branch before that UI is available).

| Job | Environment | Checks |
| --- | --- | --- |
| Linux / GCC 13 | Ubuntu 24.04 | Debug build, project warnings as errors, CTest |
| Linux / Clang 18 | Ubuntu 24.04 | Debug build, project warnings as errors, CTest |
| Windows / MSVC | Windows 2022, Visual Studio 2022 x64 | Debug build, project warnings as errors, CTest |
| Linux / ASan and UBSan | Ubuntu 24.04, Clang 18 | Instrumented project targets, fail-fast sanitizer diagnostics, CTest |
| C++ formatting | Ubuntu 24.04, clang-format 18 | Check project C++ sources without modifying them |

The workflow uses read-only repository permissions, disables persistent checkout credentials, and pins the checkout action to a full commit SHA. GoogleTest remains pinned by archive hash. Runner images and OS packages receive upstream updates; this is not a fully frozen toolchain environment. Path-filtered checks may remain pending when skipped if configured as required branch checks; consider that before changing branch protection.

Project warnings use `/W4 /permissive-` on MSVC or `-Wall -Wextra -Wpedantic` on GCC/Clang. `PLAYRUNTIME_WARNINGS_AS_ERRORS` defaults to OFF for local builds and is enabled in CI. These flags are private to project targets and do not modify GoogleTest compilation.

From `PlayRuntime/`, enable strict warnings with:

```sh
cmake --preset debug -DPLAYRUNTIME_WARNINGS_AS_ERRORS=ON
cmake --build --preset debug
ctest --preset debug
```

To check formatting, install clang-format 18 and run from `PlayRuntime/`:

```sh
cmake -P cmake/CheckFormatting.cmake
```

If the formatter is not on PATH, use `cmake -DCLANG_FORMAT=/path/to/clang-format -P cmake/CheckFormatting.cmake`. The script rejects other major versions, scans only project `include`, `src`, `apps` and `tests` C++ files, and never downloads dependencies. Apply fixes with clang-format 18's `-i` option to the reported files, then rerun the check.

On Linux with Clang, configure sanitizers in a separate directory:

```sh
cmake -S . -B ../build/play-runtime/sanitizers -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -DPLAYRUNTIME_ENABLE_SANITIZERS=ON -DPLAYRUNTIME_WARNINGS_AS_ERRORS=ON
cmake --build ../build/play-runtime/sanitizers
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1 ctest --test-dir ../build/play-runtime/sanitizers --output-on-failure --no-tests=error
```

The sanitizer option deliberately supports only Linux GCC/Clang. It instruments project code, not GoogleTest itself. CLI and test executables link the sanitizer runtimes. A detected ASan/UBSan problem fails execution rather than recovering silently. Passing these checks is useful evidence, not proof of memory safety or complete behavioural coverage.

### Validation status for the CI change

- Workflow syntax and expressions checked with actionlint 1.7.12 (optional shellcheck/pyflakes integrations disabled).
- clang-format 18.1.8 check passed; a temporary formatting violation failed as expected, then was removed.
- Windows LLVM-MinGW Debug build and both smoke tests passed with warnings as errors.
- Generated compile commands confirmed strict warnings apply to the three project targets, not GoogleTest.
- Enabling sanitizers on Windows failed configuration with the intended unsupported-platform message.
- **Not yet verified:** GitHub-hosted Linux GCC/Clang, Windows MSVC and actual ASan/UBSan execution. Review the first remote run after committing and pushing before considering the CI milestone complete.