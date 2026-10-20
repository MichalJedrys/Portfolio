# Vending Machine — C++

A C++17 console application that simulates product selection, payments, and order preparation, with separate display, controller, and feeder components.

[Python variant](../archive/Vending%20Machine%20%28Python%29/README.md)

## Features

- Ten drinks and soups with optional extras.
- Coin payments, change, cancellation, and refunds.
- Maintenance menu for transaction history and cash collection, with return and shutdown options.
- Direct exit from the main menu.
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

## Quick demonstration

Select coffee (`1`), choose extras, and insert `5` to receive change. Open maintenance (`11`, demo password `HardPassword`) to review transactions or collect cash. Return to the main menu with `4` and exit with `0`.

## Next improvements

Represent money in integer units and separate simulation state from console I/O.

[Back to portfolio](../README.md)
