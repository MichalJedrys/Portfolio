# Requirements and execution contract

Status: v0.1 target contract, partially implemented. Typed external events, batch validation and logical advancement exist. Timers, full state/action semantics, trace serialization and configurable limits below remain planned. See the README for the implemented subset and validation. Numeric limits are design defaults, not measured hardware budgets.

## Scope

The first release must execute a small sensor-driven state machine, validate its script, record external events and replay the same logical result. A headless CLI must work without graphics or hardware. An optional simulator will expose sensor controls, lights, sound and replay stepping.

Out of scope: a general game engine, arbitrary code execution, recursion, loops, implicit state entry/exit actions, hot reload, networking, BLE/NFC, embedded firmware, cloud services and hard-real-time guarantees.

## Inputs and outputs

External events have a nonnegative integer timestamp in microseconds, a globally increasing session sequence ID and a typed payload:

| Event | Payload |
| --- | --- |
| ButtonPressed | None |
| ColourDetected | One of red, green, blue, yellow, white, black, unknown |
| MotionDetected | None; thresholding belongs to the adapter |

TimerExpired is internal and carries a resolved timer name and generation. It is not accepted as an external event.

Output commands are SetLight(colour) and PlaySound(cue). Light values additionally include off and cyan. The sample uses start, success and failure sound cues. Cue existence is validated against the program's declared resources. Commands describe intent; adapters handle delivery.

The canonical logical trace records ordered commands, actual state changes and deterministic core errors. Every record has a logical timestamp and ordinal. Diagnostics about physical devices are separate.

## Time and event ordering

- Logical time uses checked nonnegative 64-bit signed integer microseconds. Script durations use positive integer milliseconds, converted with overflow checks.
- The host submits complete batches of external events and then calls advance_to(horizon). The horizon is inclusive. All events at or below it are processed before the call returns.
- After a successful advance_to, the horizon is closed. Further submissions at or before it are rejected. Before the first advance, timestamp zero is allowed. A second advance to the same horizon is an allowed no-op; backward advance is rejected.
- External sequence IDs are unique and strictly increasing in submitted order. Accepted events are processed by timestamp, then sequence ID. Submitting a batch is atomic: validation or capacity failure rejects the whole batch.
- Due timers precede external events at the same timestamp. Timer ties follow creation order. An event handler completes before another event starts.
- Starting or replacing a timer gives it a new creation ordinal. Timer deadlines and time conversions are checked for overflow. Positive durations prevent same-time timer cascades.
- A long advance may be split by the host into smaller horizons. Provided the same events are submitted before their horizons close, splitting must not change the logical trace.

This batch/horizon contract prevents an event arriving later from silently rewriting already-executed history. Live adapters normalize input before submission; their late-input policy must be documented and is not a replay guarantee.

## Handler and state semantics

A program has one initial state and ordered handlers per state. The first matching handler executes; other handlers for that event do not. An unmatched event is consumed without output.

Actions execute in declaration order. Supported actions are light, sound, timer start, timer cancel and state transition. A handler permits at most one transition, as its last action. The destination state handles the next event. A self-transition produces no state-change trace record. There are no implicit entry/exit actions.

Program loading validates state references, cue references, timer names, argument types, initial state and action counts. The initial state is recorded at logical time zero. Loading a program does not itself emit light or sound commands.

## Timer lifecycle

Timers are named one-shots. Starting an active name replaces its deadline and invalidates its old generation; starting a new name consumes one active-timer slot. Cancellation of an inactive timer is a no-op. An expired timer releases its slot before its handler executes. Timers survive state changes unless cancelled explicitly. Timer storage must not grow without bound through repeated replacement; stale generations cannot accumulate indefinitely.

## Limits and error behaviour

| Resource | Initial configurable default |
| --- | ---: |
| Script UTF-8 source bytes | 65,536 |
| Identifier bytes | 64 |
| States | 64 |
| Handlers per state | 32 |
| Actions per handler | 16 |
| Declared timer names | 64 |
| Active timers | 32 |
| Queued external events | 1,024 |
| Events processed per advance call | 10,000 |

Input configuration and effective limits belong in the session record. The language grammar must remain nonrecursive or enforce an explicit parser nesting limit before accepting nested constructs.

Invalid programs and input batches are rejected before modifying runtime state. Before executing a handler, preflight the complete action sequence against timer capacity, deadline arithmetic and the action budget. A failed preflight emits one structured error, applies none of that handler's actions and stops the runtime. Previously processed events remain committed; an entire advance is not transactional.

If the per-advance event budget is exhausted with work remaining, emit a deterministic budget error and stop before the next event. This guards against long timer chains. Budget settings and advance-call horizons must be included when replaying such errors; different horizon partitioning is only trace-equivalent when no call budget is exceeded.

A stopped runtime rejects further execution until reset. Reset creates a fresh session at time zero with the same program/configuration, clears events and timers and records the initial state. Reset is a host operation, not a DSL event. It does not implicitly emit device commands: the simulator separately clears lights and stops audio. Adapter failures do not mutate core history.

Recorded events and output traces may grow with session length. Streaming or storage belongs to the host, so a bounded-core claim must exclude unbounded host recording buffers explicitly.

## Sample quest

The initial state is idle. Timers quest_timeout and cooldown_timeout are declared. Rows below define handlers; absent combinations are unmatched events.

| State | Event | Ordered actions |
| --- | --- | --- |
| idle | ButtonPressed | light cyan; sound start; start quest_timeout 3000ms; transition waiting_for_blue |
| waiting_for_blue | ColourDetected(blue) | light blue; transition waiting_for_motion |
| waiting_for_blue | TimerExpired(quest_timeout) | light red; sound failure; start cooldown_timeout 1000ms; transition cooldown |
| waiting_for_motion | MotionDetected | cancel quest_timeout; light green; sound success; start cooldown_timeout 1000ms; transition cooldown |
| waiting_for_motion | TimerExpired(quest_timeout) | light red; sound failure; start cooldown_timeout 1000ms; transition cooldown |
| cooldown | TimerExpired(cooldown_timeout) | light off; transition idle |

The quest deadline is measured from the button, not from colour detection. Motion before blue and repeated button presses while active are ignored. Restarting requires a new button event after cooldown ends.

## Worked acceptance traces

Times below are milliseconds for readability; runtime timestamps use microseconds. Each trace starts a fresh session with default limits and advances through 5000ms. Inputs use increasing sequence IDs in table order. The initial trace entry is state=idle at 0ms. Actions on each row retain the listed order; a state change is recorded after its commands.

### Trace A: successful quest

| Time | Input or internal event | Commands and resulting state |
| --- | --- | --- |
| 0 | ButtonPressed | light cyan; sound start; state waiting_for_blue; deadline 3000 |
| 500 | ColourDetected(blue) | light blue; state waiting_for_motion |
| 900 | MotionDetected | light green; sound success; state cooldown; quest timer cancelled; cooldown deadline 1900 |
| 1900 | cooldown_timeout | light off; state idle |
| 3000 | Cancelled deadline | No event and no output |

### Trace B: motion at the exact deadline

| Time | Input or internal event | Commands and resulting state |
| --- | --- | --- |
| 0 | ButtonPressed | light cyan; sound start; state waiting_for_blue; deadline 3000 |
| 500 | ColourDetected(blue) | light blue; state waiting_for_motion |
| 3000 | quest_timeout, processed first | light red; sound failure; state cooldown; cooldown deadline 4000 |
| 3000 | MotionDetected, processed second | Unmatched in cooldown; no output |
| 4000 | cooldown_timeout | light off; state idle |

This is a timeout, not success. Due timers win ties regardless of the external event sequence ID.

### Trace C: repeated button during cooldown

| Time | Input or internal event | Commands and resulting state |
| --- | --- | --- |
| 0 | ButtonPressed | light cyan; sound start; state waiting_for_blue |
| 500 | ColourDetected(blue) | light blue; state waiting_for_motion |
| 900 | MotionDetected | light green; sound success; state cooldown; cooldown deadline 1900 |
| 1000 | ButtonPressed | Unmatched; no output and no timer replacement |
| 1500 | ButtonPressed | Unmatched; no output and no timer replacement |
| 1900 | cooldown_timeout, processed first | light off; state idle |
| 1900 | ButtonPressed, processed second | light cyan; sound start; state waiting_for_blue; quest deadline 4900 |
| 4900 | quest_timeout | light red; sound failure; state cooldown; cooldown deadline 5900 |
| 5000 | End of advance horizon | Still cooldown; no additional output |

## Recording and replay guarantee

A session records schema version, exact validated script/program source, declared resources, effective configuration, normalized external events, sequence IDs and advance horizons including the final horizon. Resets start separate sessions. Internal timer events are derived, not recorded as external inputs.

Given the same supported semantic version, program, configuration, events and advance horizons, replay must produce the same canonical logical trace. This includes deterministic core errors. Physical sound timing, rendering timing, device availability and OS scheduling are excluded. Unsupported versions, truncation, invalid ordering and invalid payloads must fail explicitly rather than be silently repaired.

## Definition of v0.1 completion

The sample runs through both CLI and simulator; scripts have meaningful diagnostics; the three traces and boundary cases are automated; sessions replay exactly; supported Windows/Linux builds pass; benchmark methodology and results are published; clean-checkout build and demo steps work. These runtime completion checks remain unverified. The current subset has typed-event runtime tests and a CLI demo; the full quest, timers, states and resource budgets remain unimplemented.
