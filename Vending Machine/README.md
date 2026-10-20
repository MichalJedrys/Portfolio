# Vending Machine — C++

A C++17 console application that simulates product selection, payments, and order preparation, with separate display, controller, and feeder components.

[Python variant](../archive/Vending%20Machine%20%28Python%29/README.md)

## Features

- Ten drinks and soups with optional extras.
- Coin payments, change, cancellation, and refunds.
- Maintenance options for transaction history, cash collection, and shutdown.
- Input validation and graceful handling of end-of-input.

The simulation uses in-memory transactions, a fixed maintenance password, and floating-point monetary values.

## Code to review

- [Workflow](source/src/VendingMachine.cpp)
- [Payments and maintenance](source/src/Controller.cpp)
- [Console menus](source/src/Display.cpp)
- [Product extras](source/src/Feeder.cpp)
- [Entry point](output/src/main.cpp)

## Build and run notes

Build with CMake and a C++17 compiler, then run `VendingMachine`. Automated tests use GoogleTest; build and test instructions are available in the [repository guide](../docs/REPOSITORY_GUIDE.md).

## Next improvements

Represent money in integer units and separate simulation state from console I/O.

[Back to portfolio](../README.md)
