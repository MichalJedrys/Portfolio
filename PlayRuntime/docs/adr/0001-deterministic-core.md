# ADR 0001: Single-owner deterministic runtime core

- Date: 2026-11-16
- Status: accepted as the initial design; not yet implemented
- Scope: PlayRuntime v0.1

## Context

The project needs to execute sensor-driven behaviours with timers and reproduce them for debugging. The same behaviour must work in automated tests, a headless CLI and a desktop simulator. Physical input timing and audio delivery introduce nondeterminism that should not obscure the engine's rules.

The initial workload is a small state machine with bounded handlers. There is no evidence that parallel rule execution is necessary. Resource limits and predictable semantics are more valuable at this stage than speculative throughput improvements.

## Decision

One execution context owns and mutates the core runtime. Time is supplied explicitly as checked integer logical timestamps. External input enters through validated batches; advancement closes inclusive horizons. Due timers precede external events at equal timestamps, with stable tie-breakers. Handlers execute to completion and emit ordered typed commands.

The language front end produces immutable program tables. Host adapters own wall-clock conversion, persistence, graphics and physical delivery. The core contains none of those dependencies. Any future audio/device concurrency communicates through an explicit bounded boundary rather than sharing mutable runtime state.

Record normalized events, configuration and advance horizons so replay can reproduce both normal behaviour and deterministic budget failures. The guarantee covers logical commands, state transitions and core errors, not physical audio or display timing.

## Alternatives considered

### Threads for each sensor or rule

This may suit independently blocking device work but makes shared state and event ordering more complex. It adds synchronization without a demonstrated core performance requirement. Device acquisition may later use threads behind adapters; rule execution will remain single-owner until evidence justifies revisiting it.

### Wall-clock-driven core

This appears convenient for a live demo but makes timer tests depend on sleeps and obscures replay equivalence. A wall-clock host adapter provides convenience without coupling the engine to real time.

### Full scripting VM or existing general-purpose engine

Either could accelerate richer experiences, but both expand the conceptual surface beyond the intended small DSL and reusable runtime. Typed bounded action tables are sufficient for the first release. This is a scope decision, not a claim that custom interpreters are universally preferable.

## Consequences

Benefits: straightforward ownership, deterministic unit/integration tests, hardware-free reproduction, explicit ordering and reusable platform-independent core logic.

Costs: adapters must normalize input and manage late events; one long handler would block progress, so handler size and per-advance work are limited; one execution context may eventually limit throughput. Recording advance horizons adds format detail but makes resource-budget failures reproducible.

A deterministic core does not imply hard-real-time performance, thread-safe public APIs or reproducible physical device timing. The host must obey the submission/advancement contract.

## Revisit criteria

Revisit only when profiling shows core execution is the limiting factor, a concrete hardware target requires another scheduling model, or a necessary language feature cannot fit bounded action tables. Preserve existing trace semantics or version the change explicitly.

## Related documents

- [Requirements and acceptance traces](../requirements.md)
- [Architecture](../architecture.md)
