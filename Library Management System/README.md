# Library Management System

A C++17 and SQLite learning project implementing a console book catalogue.

[Back to portfolio](../README.md)

## Implemented scope

- Add books with a title, author, genre, and ISBN.
- List, update, and delete stored books through a console menu.
- Store records in SQLite, using prepared statements for book mutations.
- GoogleTest cases exercise add, view, update, and delete operations against a test database.

Login, member management, borrowing, returns, fines, search, and reporting are future ideas, not implemented application features. User-related source files exist but are not included in the current library build target.

## Code to review

- [Console menu](source/src/Menu.cpp)
- [Book table operations](source/src/Table.cpp) and [SQLite wrapper](source/include/SQLiteWrapper.hpp)
- [Book model](source/include/Book.hpp)
- [Database tests](test/src/BookUnitTests.cpp)
- [Entry point](output/src/main.cpp)

## Build and run notes

Requires CMake 3.10 or later and a C++17 compiler. SQLite source and GoogleTest are bundled under `3rd_party/`. See the [repository guide](../docs/REPOSITORY_GUIDE.md) for build commands and the existing GoogleTest build caveat. A fresh build has not been verified in this documentation pass.

The executable target is `LibraryManagementSystem`. It opens or creates `LibraryDatabase.db` in its working directory. Run it from a scratch directory to preserve the committed sample database. Tests use a database named `TestDatabase` in their working directory.

## Next improvements

Verify reproducible builds and tests, strengthen input validation and database error handling, and extend tests before adding member or loan workflows.
