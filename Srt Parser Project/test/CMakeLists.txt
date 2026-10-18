include(GoogleTest)
include(CTest)

# Add the executable for the tests
add_executable(unit_tests
    src/UnitTests.cpp
)

# Include directories
target_include_directories(unit_tests PUBLIC
    include
)

# Link libraries
target_link_libraries(unit_tests PUBLIC
    srt_parser_lib
    gtest_main
    gmock_main
)

gtest_discover_tests(unit_tests XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results/unit_tests)

add_test(NAME SrtParserCli.Help COMMAND SrtParser --help)
set_tests_properties(SrtParserCli.Help PROPERTIES PASS_REGULAR_EXPRESSION "Usage:")

add_test(NAME SrtParserCli.InvalidArguments COMMAND SrtParser input.srt)
set_tests_properties(SrtParserCli.InvalidArguments PROPERTIES WILL_FAIL TRUE)