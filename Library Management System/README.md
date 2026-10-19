# Library Management System

A C++17 console catalogue backed by SQLite, with prepared statements for book operations and automated regression tests for data integrity and error handling.

## Features

- Add, list, update, and delete books with a title, author, genre, and ISBN.
- Preserve records and book IDs across application launches.
- Reject duplicate ISBNs without replacing existing books, and report missing IDs on update or delete.
- Validate console input, require title/author/ISBN, allow an empty genre, and exit cleanly on EOF.

## Build and test

Requires CMake, a C++17 compiler, and a compatible build tool. SQLite and GoogleTest source are bundled under `3rd_party/`.

From the repository root, with MinGW-w64 and CMake on PATH on Windows:

```powershell
cmake -S "Library Management System" -B build/library -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build/library --parallel 4
ctest --test-dir build/library --output-on-failure --parallel 4
```

A clean build and all 20 tests have been verified with GCC/MinGW-w64 on Windows. The executable is produced at `Library Management System/output/LibraryManagementSystem.exe`. Current CMake configuration writes the executable into the source tree and imports GoogleTest libraries from fixed `.a` paths; MSVC builds have not been verified.

## Run

Open or create a database at an explicit path:

```powershell
& ".\Library Management System\output\LibraryManagementSystem.exe" ".\build\library\demo-catalogue.db"
& ".\Library Management System\output\LibraryManagementSystem.exe" --help
```

Quote paths containing spaces. Relative paths are resolved from the current working directory, and the parent directory must already exist. Reuse the same database path on subsequent launches to retain the catalogue.

With no arguments, open or create `LibraryDatabase.db` in the current working directory. Use a separate demo database to preserve the committed sample.

Exit codes: `0` for a normal exit, EOF, or help; `1` for startup errors; `2` for invalid arguments. Database-operation errors are reported while the menu remains available.

## Persistence walkthrough

Use a new database filename for a clean demonstration. Launch the executable with the same path each time.

| Launch | Menu actions | Expected result |
| --- | --- | --- |
| 1 | Add two books with different ISBNs, then select **5** to exit. | Both records are saved. |
| 2 | Select **2** to list books, note their IDs, update one with **3**, then exit. | Previously saved records are available for editing. |
| 3 | List books, verify the updated title and unchanged ID, delete the other book with **4**, then exit. | The update survived restarting. |
| 4 | List books, then attempt a duplicate ISBN and an update/delete using a missing ID. Exit. | The deleted book stays absent; rejected operations report an error. |
| 5 | List books again. | The remaining book and its ID are unchanged. |

This sequence has also been checked automatically across separate processes using an isolated database, including a path containing spaces. The committed sample database was preserved.

## Test coverage

The suite contains 18 GoogleTest cases and two CLI checks. Database tests use private in-memory databases and cover CRUD operations, IDs above 255, bound-string lifetimes, duplicate ISBNs, missing IDs, nullable genres, binding/query failures, and table clearing. Menu tests cover malformed input, required fields, and EOF during incomplete operations.

## Implementation

- [Console menu and input handling](source/src/Menu.cpp)
- [Book operations](source/src/Table.cpp) and [SQLite wrapper](source/include/SQLiteWrapper.hpp)
- [Book model](source/include/Book.hpp)
- [Database and menu tests](test/src/BookUnitTests.cpp)
- [Entry point and database-path handling](output/src/main.cpp)

## Scope and limitations

The application implements a book catalogue. Member accounts, authentication, borrowing, returns, and fines are not implemented; the existing user-related source files are excluded from the build.

ISBN uniqueness is enforced, but ISBN format and checksum validation are not implemented. See the [repository guide](../docs/REPOSITORY_GUIDE.md) for the wider directory layout and build constraints.

[Back to portfolio](../README.md)
