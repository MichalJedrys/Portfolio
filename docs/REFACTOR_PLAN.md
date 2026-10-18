# Portfolio refactor

## Professional introduction

The main README introduces the author's C++ experience using the supplied CV. Professional experience and personal portfolio projects are described separately. No employer implementation, production benchmark, or personal project origin is implied.

## Project map

| Existing path | Classification | Development destination |
| --- | --- | --- |
| `Srt Parser Project/` | Showcase to refactor first | `projects/srt-parser/` |
| `Library Management System/` | Supporting database project | Keep at its existing path for now |
| `Snake Game/` | Earlier exercise | Linked from `archive/README.md` |
| `Vending Machine/` | Earlier exercise | Linked from `archive/README.md` |
| `Vending Machine (Python)/` | Earlier exercise | Linked from `archive/README.md` |
| New work | Independent C++20 scheduling simulator | `projects/scheduling-simulator/` |

The archive is a navigation index during this transition. No original files are relocated or deleted. The new parser implementation must identify its relationship to the original; it must not be presented as an old project imported from an invented location.

## Implementation sequence

1. Update introduction and navigation on `develop`.
2. Add the refactored parser with a standalone CMake build, tests, command-line input/output, and examples while retaining the original implementation.
3. Add a deterministic C++20 simulator with documented scheduling policies, metrics, and tests. This is new portfolio work, not a recreation of an employer's scheduler.
4. Verify available checks and document any unverified toolchains. Review the final branch before an explicitly approved release to `main`.

History is preserved through normal commits. No backdating, rewriting existing commits, or fabricated migration history is part of this work.
