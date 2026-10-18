# SRT Parser

A C++17 learning project to read subtitle input and produce text describing when displayed subtitle content changes, including overlapping lines.

[Back to portfolio](../README.md)

## Code to review

- [Parsing and timeline arrangement](source/src/SrtBase.cpp)
- [File input/output](source/src/SrtParserRealFile.cpp)
- [In-memory test adapter](source/src/SrtParserFakeFile.cpp)
- [GoogleTest cases](test/src/UnitTests.cpp)
- [Sample input](output/TestInput.srt) and [expected sample output](output/TestOutput.txt)
- [Entry point](output/src/main.cpp)

## Existing tests and limitations

Cases cover example input, empty input, missing trailing blank lines, incorrect record numbers, nested and intersecting subtitles, malformed timestamps, and a larger generated input. Their presence does not imply complete SRT format support or a measured performance guarantee.

The algorithm samples the timeline in 0.1-second steps. The missing-trailing-blank-lines test records the current behavior of dropping the unfinished final record; it does not demonstrate recovery of that record. Format validation and end-of-file handling remain to be improved.

## Build and run notes

Requires CMake 3.10 or later and a C++17 compiler; GoogleTest is bundled. See the [repository guide](../docs/REPOSITORY_GUIDE.md) for build commands and the existing GoogleTest build caveat. A fresh build has not been verified as part of this README update.

The executable target is `SrtParser`. Its entry point reads `TestInput.srt` and writes `Output.txt` relative to the working directory; it does not accept command-line file paths. Copy the sample input to a scratch directory and launch the built executable there to preserve committed output. The error path uses the Windows console command `system("pause")`.
