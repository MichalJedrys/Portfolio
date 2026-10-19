include(GoogleTest)
include(CTest)

add_executable(vending_order_tests
    src/OrderTests.cpp
    src/PaymentTests.cpp
)

target_link_libraries(vending_order_tests PUBLIC
    vending_machine_lib
    gtest_main
    gmock_main
)

gtest_discover_tests(vending_order_tests
    XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results/vending_order_tests
    PROPERTIES TIMEOUT 15
)
