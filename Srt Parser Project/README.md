# SRT Parser

A C++17 subtitle-processing tool to convert SRT cues into a timeline of displayed text. Overlapping cues are combined, with an output event whenever the displayed content changes.

## Example

Input:

```srt
1
00:00:01,000 --> 00:00:02,000
Hello.
```

Output:

```text
at 1,00 show 'Hello.'
at 2,00 show ''
```

The empty text marks the point where the subtitle disappears. Multiline content, punctuation, and spaces are preserved.

See the [sample input](output/TestInput.srt) and [expected output](output/TestOutput.txt) for overlapping cues.

## Build and test

Requires CMake, a C++17 compiler, and a compatible build tool. GoogleTest is bundled under `3rd_party/`.

From the repository root, with MinGW-w64 and CMake on PATH on Windows:

```powershell
cmake -S "Srt Parser Project" -B build/srt -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build/srt --parallel 4
ctest --test-dir build/srt --output-on-failure
```

The build and all 22 tests have been verified with GCC/MinGW-w64 on Windows. The executable is produced at `Srt Parser Project/output/SrtParser.exe`. Current CMake configuration writes the executable into the source tree and imports GoogleTest libraries from fixed `.a` paths; MSVC builds have not been verified.

## Run

Pass the input and output paths, quoting paths that contain spaces:

```powershell
& ".\Srt Parser Project\output\SrtParser.exe" ".\Srt Parser Project\output\TestInput.srt" ".\build\srt\sample-timeline.txt"
& ".\Srt Parser Project\output\SrtParser.exe" --help
```

Output files are overwritten, and the destination directory must already exist. Input and output paths referring to the same file are rejected.

With no arguments, read `TestInput.srt` and write `Output.txt` in the current working directory. Supply both paths to use another location.

Exit codes: `0` for success or help, `1` for processing or file errors, and `2` for an incorrect argument count. Errors are written to standard error.

## Test coverage

The suite contains 20 GoogleTest cases and two CLI checks. Coverage includes overlapping and nested cues, malformed input, final cues without trailing newlines, 5,000 sequential records, parser reuse, text preservation, and file-read failures. The large-input case verifies correctness rather than execution speed.

Additional executable checks cover paths containing spaces, default paths, missing input, invalid output, and protection against overwriting the input file.

## Implementation

- [Parsing and timeline processing](source/src/SrtBase.cpp)
- [File input/output](source/src/SrtParserRealFile.cpp)
- [In-memory test adapter](source/src/SrtParserFakeFile.cpp)
- [Unit tests](test/src/UnitTests.cpp) and [CLI checks](test/CMakeLists.txt)
- [Command-line entry point](output/src/main.cpp)

## Current limitations

- Timeline processing uses 100 ms sampling. Short cues may be omitted, and exact millisecond boundaries are not preserved.
- Cue IDs must start at 1 and remain consecutive. Malformed records may be skipped without diagnostics; timestamp ranges and input encoding require further validation.
- Output is a text-event timeline, with comma-formatted timestamps and two decimal places. SRT/WebVTT export is not provided.

[Back to portfolio](../README.md)
