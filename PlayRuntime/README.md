# PlayRuntime

PlayRuntime is a planned C++20 interaction engine that turns sensor events into scripted light and sound commands. A desktop simulator will let users create, record and replay interactions without physical hardware.

## Status

The project currently contains design documents only. There is no executable, build configuration, scripting implementation or measured performance yet. Development takes place on `develop-play-runtime`.

## Example interaction

A button starts a three-second quest. Detecting blue and then motion completes it with a green light and success sound. A timeout produces a red light and failure sound. Both outcomes enter a one-second cooldown before another quest can start.

The engine will own states, timers and rule execution. A small domain-specific language will describe behaviours without recompiling C++. Host adapters will supply sensor events and consume commands. Recording and replay will reproduce the logical interaction for debugging; they will not promise identical physical audio timing.

## First release

- A reusable deterministic core with explicit resource limits.
- A small validated scripting language and headless CLI.
- Versioned session recording and exact logical trace replay.
- An optional desktop simulator with virtual sensors and sound cues.
- Automated correctness checks and reproducible performance measurements.

Physical sensors, wireless protocols and firmware deployment are later extensions. This is an independent portfolio project, not a LEGO product or an implementation of a LEGO protocol.

## Design documents

- [Requirements and execution contract](docs/requirements.md): scope, semantics, resource limits, example behaviour and acceptance traces.
- [Architecture](docs/architecture.md): components, ownership and data flow.
- [ADR 0001](docs/adr/0001-deterministic-core.md): the decision to use a single-owner deterministic core.

## Next milestone

Introduce an isolated CMake build, a C++20 core target and a source-built GoogleTest dependency. Build instructions will be added once the commands actually work.
