cmake_minimum_required(VERSION 3.13)

# Set the project name
project(VendingMachine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(source)
add_subdirectory(output)

# Enable order regression tests.
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(3rd_party)
    add_subdirectory(test)
endif()
