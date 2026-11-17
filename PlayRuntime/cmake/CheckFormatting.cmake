cmake_minimum_required(VERSION 3.24)

# Run without configuring or fetching the project: cmake -P cmake/CheckFormatting.cmake
if(NOT DEFINED CLANG_FORMAT)
    find_program(CLANG_FORMAT NAMES clang-format-18 clang-format REQUIRED)
endif()
execute_process(COMMAND "${CLANG_FORMAT}" --version
    RESULT_VARIABLE result OUTPUT_VARIABLE version ERROR_VARIABLE error)
if(NOT result EQUAL 0 OR NOT version MATCHES "version 18\\.")
    message(FATAL_ERROR "clang-format 18 is required. Got: ${version}${error}")
endif()
get_filename_component(project_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(sources)
foreach(directory include src apps tests)
    file(GLOB_RECURSE directory_sources
        "${project_root}/${directory}/*.cpp"
        "${project_root}/${directory}/*.hpp")
    list(APPEND sources ${directory_sources})
endforeach()
if(NOT sources)
    message(FATAL_ERROR "No PlayRuntime C++ files found")
endif()
foreach(source IN LISTS sources)
    execute_process(COMMAND "${CLANG_FORMAT}" --dry-run --Werror "${source}"
        RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Formatting failed for ${source}:\n${output}${error}")
    endif()
endforeach()
list(LENGTH sources count)
message(STATUS "Formatting passed for ${count} PlayRuntime C++ files")
