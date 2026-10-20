# Vending Machine — C++

A C++17 console simulation with display, controller, and feeder components.

[Python variant](../archive/Vending%20Machine%20%28Python%29/README.md)

## Implemented scope

- Ten products: hot drinks and soups, with milk, sugar, or soy sauce options depending on the selection.
- Simulated coin payments, cancellation with refunds by entering zero, and change calculation. Invalid coins are rejected without losing accepted credit.
- Maintenance options for session transactions, resetting collected money, and shutdown.
- Line-based input validation across menus, extras, and payments; EOF exits the application and refunds any incomplete payment.
- Exceptions for several invalid menu choices and payment values.

The maintenance password is hardcoded, transactions are held in memory, and money uses floating-point values. Entering zero cancels payment; an actual timed payment workflow is not implemented.

## Code to review

- [Workflow](source/src/VendingMachine.cpp)
- [Payments and maintenance](source/src/Controller.cpp)
- [Console menus](source/src/Display.cpp)
- [Product extras](source/src/Feeder.cpp)
- [Entry point](output/src/main.cpp)

## Build and run notes

Requires CMake 3.13 or later and a C++17 compiler. The executable target is `VendingMachine`, with output configured under `output/`. See the [repository guide](../docs/REPOSITORY_GUIDE.md) for build commands. The build and 40 CTest checks (39 GoogleTest cases and one executable EOF check) have been verified with GCC/MinGW-w64 on Windows. The checks cover orders, refunds, change, input recovery, malformed values, and EOF across all prompts. GoogleTest is bundled under `3rd_party/google_test`, matching the other projects.

## Next improvements

Extend payment and input-validation tests, represent money in integer units, and separate simulation state from console I/O before extending the interface.

[Back to portfolio](../README.md)
