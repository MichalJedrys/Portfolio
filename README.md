# Michał Jędrys Portfolio

C++ projects exploring interaction-engine design, application architecture, data persistence, text processing, and automated testing. Each project addresses a distinct engineering problem, with source code, build configuration, and documentation covering implementation details and current limitations.

## Projects

| Project | Technologies | Engineering focus |
| --- | --- | --- |
| [PlayRuntime](PlayRuntime/README.md) — in development | C++20, CMake, GoogleTest, GitHub Actions | Sensor-driven interaction engine with a documented deterministic execution model. Implements typed event dispatch, logical time advancement and a programmatic button-to-light demo, with tests and CI configuration. |
| [Library Management System](Library%20Management%20System/README.md) | C++17, SQLite, GoogleTest | Book catalogue with persistent storage, prepared statements, and database tests for create, read, update, and delete operations. |
| [SRT Parser](Srt%20Parser%20Project/README.md) | C++17, STL, GoogleTest | Subtitle parsing and timeline processing, with tests covering overlapping subtitles, malformed input, and boundary cases. |
| [Vending Machine](Vending%20Machine/README.md) | C++17, CMake | Console simulation separating display, controller, and feeder components, with product selection, payment handling, change calculation, and maintenance operations. |

## PlayRuntime

[PlayRuntime](PlayRuntime/README.md) is an interaction-engine project intended to turn sensor events into scripted light and sound behaviours. Its design separates a platform-independent C++ core from device adapters, with explicit event ordering, timers, state transitions, and resource limits.

The planned showcase is a desktop simulation of a colour-and-motion quest, with session recording and deterministic replay for debugging. The current implementation provides a C++20 core with typed events, atomic input batches, deterministic dispatch, and a CLI button-to-light demonstration with automated tests. GitHub Actions is configured for Linux GCC/Clang, Windows MSVC, formatting, and Linux ASan/UBSan checks. Timers, state machines, scripting, session replay, and the simulator are still planned features.

Development takes place on `develop-play-runtime`. See the [project README](PlayRuntime/README.md) for build instructions and validation status, the [execution contract](PlayRuntime/docs/requirements.md) for acceptance scenarios, and the [architecture](PlayRuntime/docs/architecture.md) for component responsibilities and design decisions.

## Code and build documentation

Each project has an independent CMake configuration and executable entry point. Project READMEs provide links to the core implementation, build and runtime requirements, test coverage where available, and areas for further development.

See the [repository guide](docs/REPOSITORY_GUIDE.md) for the directory structure, build commands, and known build constraints.

## Archive

Earlier projects are retained in [archive/](archive/) for reference.
