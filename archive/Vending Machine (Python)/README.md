# Vending Machine — Python

A Python variant of the [C++ vending-machine exercise](../../Vending%20Machine/README.md), with display, controller, feeder, and orchestration classes.

## Code to review

- [Entry point](source/main.py)
- [Workflow](source/VendingMachine.py)
- [Payments and catalogue](source/Controller.py)
- [Display](source/Display.py) and [extras](source/Feeder.py)

The console workflow includes product selection, simulated payments and change, and maintenance options. It uses a hardcoded demo password and session-only transaction history.

## Run

Requires Python 3.10 or later because the source uses `match` statements. From the repository root:

```powershell
python "archive/Vending Machine (Python)/source/main.py"
```

The source uses standard-library modules. Direct execution avoids the existing CMake helper, which creates a virtual environment and attempts package upgrades during configuration using a Unix-style environment path.

## Current limitations

The payment reader uses `int()` even though the menu advertises fractional coins; fractional payments are therefore not handled correctly. Input handling and money representation remain to be improved, and automated tests remain to be added. No project-owned automated tests are present, and interactive execution has not been verified as part of this README update.

[Back to portfolio](../../README.md)
