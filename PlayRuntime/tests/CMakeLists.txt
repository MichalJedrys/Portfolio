include(FetchContent)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
# Keep GoogleTest and the project on the same MSVC runtime.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    URL_HASH SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(playruntime_tests build_smoke_test.cpp)
playruntime_configure_target(playruntime_tests)
target_link_libraries(playruntime_tests PRIVATE playruntime::core GTest::gtest_main)
target_compile_definitions(playruntime_tests PRIVATE EXPECTED_PROJECT_VERSION="${PROJECT_VERSION}")
include(GoogleTest)
gtest_discover_tests(playruntime_tests PROPERTIES TIMEOUT 10)

add_test(NAME CliSmoke COMMAND "${CMAKE_COMMAND}"
    "-DCLI=$<TARGET_FILE:playruntime>"
    "-DEXPECTED_VERSION=${PROJECT_VERSION}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/cli_smoke.cmake")
set_tests_properties(CliSmoke PROPERTIES TIMEOUT 30)
