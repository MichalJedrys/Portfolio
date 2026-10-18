# Michał Jędrys | C++ Portfolio

Learning projects to explore C++, object-oriented design, SQLite persistence, parsing, and console applications.

## Projects

| Project | Focus | Current scope |
| --- | --- | --- |
| [Library Management System](Library%20Management%20System/README.md) | C++17, SQLite, GoogleTest | Console book catalogue with add, list, update, and delete operations and database tests. |
| [SRT Parser](Srt%20Parser%20Project/README.md) | C++17, STL, GoogleTest | Converts subtitles into timestamped text changes; tests include overlaps and malformed input. |
| [Snake Game](archive/Snake%20Game/README.md) | C++11, Windows console API | Keyboard controls, scoring, collision detection, and restart support. |
| [Vending Machine — C++](Vending%20Machine/README.md) | C++17, console simulation | Product selection, simulated coin payments and change, and maintenance options. |
| [Vending Machine — Python](archive/Vending%20Machine%20%28Python%29/README.md) | Python 3 | A Python variant of the vending-machine exercise. |

Start with **Library Management System** for database interaction or **SRT Parser** for parsing logic and test cases. Current limitations are documented in the individual READMEs.

## Explore the code

Each project is independent and has its own entry point. The C++ projects have separate CMake configurations; there is no repository-wide build target.

See the [repository guide](docs/REPOSITORY_GUIDE.md) for the directory layout, build caveats, and development/release workflow.
