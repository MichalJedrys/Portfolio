cmake_minimum_required(VERSION 3.24)

project(PlayRuntime VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Use an out-of-source build, for example: cmake -S . -B ../build/play-runtime")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
configure_file(cmake/version_config.hpp.in generated/version_config.hpp @ONLY)

add_library(playruntime_core STATIC src/core/version.cpp)
add_library(playruntime::core ALIAS playruntime_core)
target_compile_features(playruntime_core PUBLIC cxx_std_20)
target_include_directories(playruntime_core
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

add_executable(playruntime apps/cli/main.cpp)
target_link_libraries(playruntime PRIVATE playruntime::core)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
