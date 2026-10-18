# Michał Jędrys | C++ Software Engineer

I am a C++ software engineer with over three years of professional experience in Linux-based real-time and embedded systems across telecommunications and automotive software. My work includes modern C++17/20, performance-sensitive algorithms, unit and integration testing, and code review.

My professional experience includes 5G Layer 2 packet-scheduling software at Nokia and embedded automotive software at VESOFTx. This repository contains personal projects; it does not contain employer code.

## Engineering focus

- Modern C++ and maintainable interfaces.
- Deterministic behavior, algorithm design, and performance analysis.
- Linux and embedded systems, with automated tests and reproducible builds.

## Portfolio navigation

See [current projects](projects/README.md) for the showcase work and [earlier projects](archive/README.md) for the original console exercises. The earlier projects document my learning history; they are not a substitute for my professional experience.

## Projects

| Project | Focus | Current scope |
| --- | --- | --- |
| [SRT Parser — refactored](projects/srt-parser/README.md) | C++17, CMake, CTest | Standalone library and CLI, exact millisecond boundaries, and regression tests. |
| [Scheduling simulator](projects/scheduling-simulator/README.md) | C++20, deterministic simulation | Round-robin and preemptive EDF, execution timelines, per-job metrics, and tests. |
| [Library Management System](Library%20Management%20System/README.md) | C++17, SQLite, GoogleTest | Console book catalogue with add, list, update, and delete operations and database tests. |
| [SRT Parser — original](Srt%20Parser%20Project/README.md) | C++17, STL, GoogleTest | Original implementation retained for reference and history. |
| [Snake Game](Snake%20Game/README.md) | C++11, Windows console API | Keyboard controls, scoring, collision detection, and restart support. |
| [Vending Machine — C++](Vending%20Machine/README.md) | C++17, console simulation | Product selection, simulated coin payments and change, and maintenance options. |
| [Vending Machine — Python](Vending%20Machine%20%28Python%29/README.md) | Python 3 | A Python variant of the vending-machine exercise. |

Start with the **refactored SRT Parser** for parsing and tests, or the **scheduling simulator** for algorithm design and metrics. The library project provides a further example of database interaction. Project READMEs distinguish implemented behavior from limitations and planned work.

## Explore the code

Each project is independent and has its own entry point. The C++ projects have separate CMake configurations; there is no repository-wide build target.

The two projects under `projects/` use standalone CMake builds and dependency-free CTest suites. The [C++ checks](https://github.com/MichalJedrys/Portfolio/actions/workflows/portfolio-cpp.yml) build and test them on Linux and Windows. The older projects have separate toolchain caveats documented in their READMEs.

See the [repository guide](docs/REPOSITORY_GUIDE.md) for the directory layout, build caveats, and development/release workflow.
