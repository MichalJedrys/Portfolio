include(GoogleTest)
include(CTest)

add_executable(vending_order_tests
    src/OrderTests.cpp
    src/PaymentTests.cpp
    src/InputTests.cpp
)

add_test(NAME VendingCli.EndOfInput
    COMMAND ${CMAKE_COMMAND} -DPROGRAM=$<TARGET_FILE:VendingMachine>
        -P ${CMAKE_CURRENT_SOURCE_DIR}/EndOfInput.cmake)
set_tests_properties(VendingCli.EndOfInput PROPERTIES TIMEOUT 30)

target_link_libraries(vending_order_tests PUBLIC
    vending_machine_lib
    gtest_main
    gmock_main
)

gtest_discover_tests(vending_order_tests
    XML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results/vending_order_tests
    PROPERTIES TIMEOUT 15
)
