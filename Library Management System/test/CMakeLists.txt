include(GoogleTest)
include(CTest)

# Add the executable for the tests
add_executable(book_test
    src/BookUnitTests.cpp
)

# Include directories
target_include_directories(book_test PUBLIC
    include
)

# Link libraries
target_link_libraries(book_test PUBLIC
    sqlite_lib
    library_management_system_lib
    gtest_main
    gmock_main
)

gtest_discover_tests(book_test XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results/book_test PROPERTIES TIMEOUT 10)

add_test(NAME LibraryCli.Help COMMAND LibraryManagementSystem --help)
set_tests_properties(LibraryCli.Help PROPERTIES PASS_REGULAR_EXPRESSION "Usage:" TIMEOUT 10)

add_test(NAME LibraryCli.InvalidArguments COMMAND LibraryManagementSystem first.db extra.db)
set_tests_properties(LibraryCli.InvalidArguments PROPERTIES WILL_FAIL TRUE TIMEOUT 10)