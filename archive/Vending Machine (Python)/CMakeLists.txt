# Minimum version of CMake required
cmake_minimum_required(VERSION 3.11)

# Set the project name
project(VendingMachine)

# Set Python version to use
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Set Python source files as a part of the project
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Controller.py
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Display.py
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Feeder.py
    ${CMAKE_CURRENT_SOURCE_DIR}/source/VendingMachine.py
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.py
)

# Specify the main Python script
set(MAIN_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/source/main.py)

# Add a custom target to run the main script
add_custom_target(run_vending_machine ALL
    COMMAND ${Python3_EXECUTABLE} ${MAIN_SCRIPT}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${SOURCE_FILES}
)

# Set up a virtual environment for Python
execute_process(
    COMMAND ${Python3_EXECUTABLE} -m venv venv
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Activate the virtual environment and install any requirements
execute_process(
    COMMAND venv/bin/python -m pip install --upgrade pip setuptools
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)