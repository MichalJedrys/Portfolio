# Vending Machine — C++

A C++17 console simulation with display, controller, and feeder components.

[Python variant](../Vending%20Machine%20%28Python%29/README.md)

## Implemented scope

- Ten products: hot drinks and soups, with milk, sugar, or soy sauce options depending on the selection.
- Simulated coin payments, cancellation by entering zero, and change calculation.
- Maintenance options for session transactions, resetting collected money, and shutdown.
- Exceptions for several invalid menu choices and payment values.

This is a learning simulation. Its maintenance password is hardcoded, transactions are held in memory, and money uses floating-point values. Entering zero simulates a payment timeout; an actual timed payment workflow is not implemented.

## Code to review

- [Workflow](source/src/VendingMachine.cpp)
- [Payments and maintenance](source/src/Controller.cpp)
- [Console menus](source/src/Display.cpp)
- [Product extras](source/src/Feeder.cpp)
- [Entry point](output/src/main.cpp)

## Build and run notes

Requires CMake 3.10 or later and a C++17 compiler. The executable target is `VendingMachine`, with output configured under `output/`. See the [repository guide](../docs/REPOSITORY_GUIDE.md) for build commands. A fresh build and interactive execution have not been verified as part of this README update. No project-owned automated tests are present.

## Next improvements

Add payment and input-validation tests, represent money in integer units, and separate simulation state from console I/O before extending the interface.

[Back to portfolio](../README.md)
