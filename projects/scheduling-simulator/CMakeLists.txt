cmake_minimum_required(VERSION 3.20)
project(SchedulingSimulator VERSION 1.0.0 LANGUAGES CXX)

add_library(scheduler src/scheduler.cpp)
target_include_directories(scheduler PUBLIC include)
target_compile_features(scheduler PUBLIC cxx_std_20)
set_target_properties(scheduler PROPERTIES CXX_EXTENSIONS OFF)
if(MSVC)
    target_compile_options(scheduler PRIVATE /W4 /permissive-)
else()
    target_compile_options(scheduler PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(scheduling-simulator src/main.cpp)
target_link_libraries(scheduling-simulator PRIVATE scheduler)

include(CTest)
if(BUILD_TESTING)
    add_executable(scheduler-tests tests/scheduler_tests.cpp)
    target_link_libraries(scheduler-tests PRIVATE scheduler)
    add_test(NAME scheduler-regressions COMMAND scheduler-tests)
    add_test(NAME cli-help COMMAND scheduling-simulator --help)
    foreach(policy IN ITEMS rr edf)
        add_test(NAME cli-${policy}-example COMMAND ${CMAKE_COMMAND}
            -DPROGRAM=$<TARGET_FILE:scheduling-simulator>
            -DPOLICY=${policy}
            -DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/examples/jobs.csv
            -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/examples/${policy}.txt
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_example.cmake)
    endforeach()
    add_test(NAME cli-invalid-policy COMMAND scheduling-simulator unknown missing.csv)
    set_tests_properties(cli-invalid-policy PROPERTIES WILL_FAIL TRUE)
endif()
