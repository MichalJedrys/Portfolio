cmake_minimum_required(VERSION 3.20)
project(PortfolioSrtParser VERSION 1.0.0 LANGUAGES CXX)

add_library(srt_parser src/parser.cpp)
target_include_directories(srt_parser PUBLIC include)
target_compile_features(srt_parser PUBLIC cxx_std_17)
set_target_properties(srt_parser PROPERTIES CXX_EXTENSIONS OFF)
if(MSVC)
    target_compile_options(srt_parser PRIVATE /W4 /permissive-)
else()
    target_compile_options(srt_parser PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(srt-parser src/main.cpp)
target_link_libraries(srt-parser PRIVATE srt_parser)

include(CTest)
if(BUILD_TESTING)
    add_executable(srt-parser-tests tests/parser_tests.cpp)
    target_link_libraries(srt-parser-tests PRIVATE srt_parser)
    add_test(NAME parser-regressions COMMAND srt-parser-tests)
    add_test(NAME cli-help COMMAND srt-parser --help)
    add_test(NAME cli-example COMMAND ${CMAKE_COMMAND}
        -DPROGRAM=$<TARGET_FILE:srt-parser>
        -DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/examples/overlap.srt
        -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/examples/overlap.txt
        -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/example-output.txt
        -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_example.cmake)
    add_test(NAME cli-missing-input COMMAND srt-parser missing-input.srt)
    set_tests_properties(cli-missing-input PROPERTIES WILL_FAIL TRUE)
endif()
