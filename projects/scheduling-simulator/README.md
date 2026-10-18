# Scheduling simulator

A new, independent C++20 portfolio project for comparing deterministic single-CPU scheduling policies. It produces an execution timeline and per-job metrics from a small CSV workload.

[Back to portfolio](../../README.md)

This is an educational scheduling model written for this portfolio. It does not contain employer code, model a 5G scheduler, or claim production real-time guarantees.

## Policies and model

- **Round-robin (`rr`)** uses a configurable positive quantum, defaulting to 2 time units. New arrivals, including arrivals exactly at a quantum boundary, join the queue before the preempted job is requeued.
- **Preemptive earliest-deadline-first (`edf`)** selects the ready job with the earliest absolute deadline, reconsidering the decision at arrivals and completions. Equal deadlines use arrival time, then CSV order.

The model has one CPU, independent jobs, integer time, known execution durations, no I/O blocking, and zero dispatch/context-switch overhead. Missed-deadline jobs continue running. No OS threads, wall-clock sleeps, or random inputs are used. Adjacent timeline segments for the same job are combined.

## Build and test

Requires CMake 3.20 or later and a C++20 compiler supporting `std::span`. No external dependencies are downloaded.

From the repository root:

```sh
cmake -S projects/scheduling-simulator -B build/scheduling-simulator -DCMAKE_BUILD_TYPE=Release
cmake --build build/scheduling-simulator --config Release
ctest --test-dir build/scheduling-simulator -C Release --output-on-failure
./build/scheduling-simulator/scheduling-simulator rr projects/scheduling-simulator/examples/jobs.csv 2
./build/scheduling-simulator/scheduling-simulator edf projects/scheduling-simulator/examples/jobs.csv
```

For Visual Studio, use `./build/scheduling-simulator/Release/scheduling-simulator.exe` instead. Single-configuration Windows generators use the build-directory executable with `.exe`. `--help` prints usage. Argument-count errors return 2; workload, policy, or I/O errors return 1; success returns 0.

## Workload and results

The CSV header is exactly `id,arrival,duration,deadline`. Fields are unquoted with no extra whitespace; blank records are rejected. IDs use ASCII letters, digits, underscores, or hyphens and must be unique. Arrival and absolute deadline range from 0 to 1,000,000,000; duration and RR quantum range from 1 to 1,000,000,000. At most 10,000 jobs and 1,000,000 scheduling steps are allowed. These bounds protect arithmetic and stop impractically fine RR simulations. A deadline before arrival is permitted and will be reported as missed. A header-only workload is valid.

For the [sample workload](examples/jobs.csv), the [round-robin report](examples/rr.txt) has one missed deadline; the [EDF report](examples/edf.txt) has none. This is an illustrative workload, not a claim that EDF is always preferable or that it minimizes every metric.

| Metric | Definition |
| --- | --- |
| Response | First execution time minus arrival |
| Turnaround | Completion minus arrival |
| Waiting | Turnaround minus execution duration |
| Deadline missed | Completion is strictly later than the absolute deadline |
| CPU utilization | Busy time divided by makespan, measured from time zero |
| Throughput | Completed jobs divided by makespan, in jobs per time unit |

Summary means are over all jobs. Empty workloads report zero summary metrics. Ratios use three decimal places. Time units are abstract; these are simulated outcomes, not measured CPU performance.

## Design and validation

[`scheduler.hpp`](include/scheduling/scheduler.hpp) separates workload parsing, simulation, and reporting. [`scheduler.cpp`](src/scheduler.cpp) advances to arrivals, completions, or quantum boundaries instead of simulating every tick.

The [regression suite](tests/scheduler_tests.cpp) checks known schedules, preemption, arrival-boundary ordering, ties, idle intervals, unsorted inputs, deadline equality, invalid data, determinism, and conservation of execution time. CTest compares both command-line reports with the committed examples. Tests remain active in Release builds.

EDF currently scans the ready queue for each decision; large ready sets are a candidate for a priority queue after profiling. RR output and work scale with the number of quanta. There is no benchmark or schedulability proof in this first version.
