# build GoogleTest

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)

include(ExternalProject)

ExternalProject_Add(
    googletest_project
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd_party/google_test
    BINARY_DIR ${CMAKE_BINARY_DIR}/3rd_party/google_test
    CMAKE_ARGS
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_CXX_FLAGS_INIT=${CMAKE_CXX_FLAGS_INIT}
        -DCMAKE_C_COMPILER_TARGET=${CMAKE_C_COMPILER_TARGET}
        -DCMAKE_CXX_COMPILER_TARGET=${CMAKE_CXX_COMPILER_TARGET}
        -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

add_library(gtest IMPORTED STATIC GLOBAL)
add_dependencies(gtest googletest_project)
set_target_properties(gtest 
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/3rd_party/google_test/lib/libgtest.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/3rd_party/google_test/googletest/include"
)

add_library(gtest_main IMPORTED STATIC GLOBAL)
add_dependencies(gtest_main googletest_project)
set_target_properties(gtest_main 
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/3rd_party/google_test/lib/libgtest_main.a
    INTERFACE_LINK_LIBRARIES gtest
)

add_library(gmock IMPORTED STATIC GLOBAL)
add_dependencies(gmock googletest_project)
set_target_properties(gmock 
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/3rd_party/google_test/lib/libgmock.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/3rd_party/google_test/googlemock/include"
)

add_library(gmock_main IMPORTED STATIC GLOBAL)
add_dependencies(gmock_main googletest_project)
set_target_properties(gmock_main 
    PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/3rd_party/google_test/lib/libgmock_main.a
    INTERFACE_LINK_LIBRARIES gmock
)

# gtest uses threading, need to add the dependency
    set_target_properties(gtest
        PROPERTIES
        INTERFACE_LINK_LIBRARIES Threads::Threads
    )
    set_target_properties(gmock
        PROPERTIES
        INTERFACE_LINK_LIBRARIES Threads::Threads
    )