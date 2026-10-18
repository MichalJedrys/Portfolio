# SRT Parser

A standalone C++17 library and command-line tool that converts subtitle cues into changes in displayed text. It handles overlapping cues using exact integer millisecond boundaries.

[Back to portfolio](../../README.md) · [Original project](../../Srt%20Parser%20Project/README.md)

## Origin and scope

This version is a new refactor of the SRT Parser already present in this repository. The original implementation, samples, tests, and Git history remain at `Srt Parser Project/`. The refactor separates parsing, timeline construction, and file I/O. It replaces the original 0.1-second sampling loop with boundary events and retains a final cue without requiring trailing blank lines.

## Build and test

Requires CMake 3.20 or later and a C++17 compiler. No external packages or network downloads are required by the build.

From the repository root:

```sh
cmake -S projects/srt-parser -B build/srt-parser -DCMAKE_BUILD_TYPE=Release
cmake --build build/srt-parser --config Release
ctest --test-dir build/srt-parser -C Release --output-on-failure
```

Single-configuration generators place the executable at `build/srt-parser/srt-parser` (or `.exe`). Visual Studio uses `build/srt-parser/Release/srt-parser.exe`.

```sh
./build/srt-parser/srt-parser projects/srt-parser/examples/overlap.srt
./build/srt-parser/srt-parser projects/srt-parser/examples/overlap.srt build/timeline.txt
```

On Visual Studio builds, use `./build/srt-parser/Release/srt-parser.exe` instead. `--help` prints usage. Exit codes are 0 for success, 1 for input/output or format errors, and 2 for invalid argument counts. Input and output cannot refer to the same existing file. Parsing finishes before an output file is opened; a write failure may still leave a partial output file.

## Format and behavior

- Accepts LF or CRLF, an optional UTF-8 BOM, and a final record without a trailing newline.
- Cue IDs are positive and strictly increasing, with gaps permitted. Timestamps use `HH:MM:SS,mmm`; minutes and seconds must be below 60 and end must be after start.
- Cues can appear out of chronological order. Active text is joined in input order; intervals are half-open, so a cue is no longer visible at its end time.
- Output has one line per text change: `at 1.005 show 'First subtitle'`. Newlines, tabs, backslashes, and apostrophes in text are escaped. Output numbers use a decimal point independently of the system locale.
- Empty input produces no changes. Malformed records stop processing with a line-number diagnostic.

This deliberately supports a documented subset of SRT: two-digit hours, no cue-position metadata, no encoding conversion, and no interpretation of markup. UTF-8 text is carried as bytes. The complete input and timeline are held in memory. Timeline construction sorts boundaries; building text for heavily overlapping cues can still be expensive. No performance benchmark is claimed.

## Design and tests

[`include/srt/parser.hpp`](include/srt/parser.hpp) exposes cue parsing, timeline construction, and formatting. [`src/main.cpp`](src/main.cpp) handles paths and error reporting.

The dependency-free [regression executable](tests/parser_tests.cpp) checks EOF handling, millisecond timing, BOM/CRLF, overlap and nesting, shared boundaries, invalid records, escaping, and output errors. CTest also checks help, missing input, and the [committed CLI example](examples/overlap.txt). These tests remain enabled in Release builds; they do not rely on `assert`.
