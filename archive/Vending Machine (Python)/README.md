# Vending Machine — Python

A Python variant of the [C++ vending machine](../../Vending%20Machine/README.md), with display, controller, feeder, and orchestration classes.

## Code to review

- [Entry point](source/main.py)
- [Workflow](source/VendingMachine.py)
- [Payments and catalogue](source/Controller.py)
- [Display](source/Display.py) and [extras](source/Feeder.py)

The console workflow includes product selection, simulated payments and change, and maintenance options. Payments support fractional coins, invalid-input retries, cancellation, and refunds using exact decimal amounts. A fixed demo password and session-only transaction history are used.

## Run

Requires Python 3.10 or later. From the repository root:

```powershell
python "archive/Vending Machine (Python)/source/main.py"
```

Only Python's standard library is required.

[Back to portfolio](../../README.md)
