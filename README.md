# Michał Jędrys Portfolio

C++ projects focused on application architecture, data persistence, text processing, and automated testing. Each project addresses a distinct engineering problem, with source code, build configuration, and documentation covering implementation details and current limitations.

## Projects

| Project | Technologies | Engineering focus |
| --- | --- | --- |
| [Library Management System](Library%20Management%20System/README.md) | C++17, SQLite, GoogleTest | Book catalogue with persistent storage, prepared statements, and database tests for create, read, update, and delete operations. |
| [SRT Parser](Srt%20Parser%20Project/README.md) | C++17, STL, GoogleTest | Subtitle parsing and timeline processing, with tests covering overlapping subtitles, malformed input, and boundary cases. |
| [Vending Machine](Vending%20Machine/README.md) | C++17, CMake | Console simulation separating display, controller, and feeder components, with product selection, payment handling, change calculation, and maintenance operations. |

## Code and build documentation

Each project has an independent CMake configuration and executable entry point. Project READMEs provide links to the core implementation, build and runtime requirements, test coverage where available, and areas for further development.

See the [repository guide](docs/REPOSITORY_GUIDE.md) for the directory structure, build commands, and known build constraints.

## Archive

Earlier projects are retained in [archive/](archive/) for reference.
