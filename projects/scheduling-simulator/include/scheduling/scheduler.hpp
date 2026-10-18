#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace scheduling {

using Time = std::int64_t;
enum class Policy { round_robin, earliest_deadline_first };

struct Job {
    std::string id;
    Time arrival;
    Time duration;
    Time deadline;
};
struct Segment {
    Time start;
    Time end;
    // Index into the input jobs; nullopt represents an idle CPU.
    std::optional<std::size_t> job;
};
struct Metrics {
    Time completion;
    Time turnaround;
    Time waiting;
    Time response;
    bool deadline_missed;
};
struct Result {
    std::vector<Segment> timeline;
    // Same order as the input jobs.
    std::vector<Metrics> jobs;
    Time busy_time = 0;
    Time makespan = 0;
};

std::vector<Job> read_jobs(std::istream& input);
// One CPU, zero context-switch cost; EDF is preemptive on job arrivals.
Result simulate(std::span<const Job> jobs, Policy policy, Time quantum = 2);
void write_report(std::ostream& output, std::span<const Job> jobs, const Result& result);

} // namespace scheduling
