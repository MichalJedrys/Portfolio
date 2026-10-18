#include "scheduling/scheduler.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using namespace scheduling;
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
void rejects(const std::function<void()>& operation) {
    try { operation(); }
    catch (const std::runtime_error&) { return; }
    throw std::runtime_error("invalid workload was accepted");
}
void invariants(const std::vector<Job>& jobs, const Result& result) {
    std::vector<Time> executed(jobs.size(), 0);
    Time previous = 0;
    Time busy = 0;
    for (const auto& segment : result.timeline) {
        require(segment.start == previous && segment.end > segment.start, "gap, overlap, or empty segment");
        previous = segment.end;
        if (segment.job) {
            const auto index = *segment.job;
            require(index < jobs.size() && segment.start >= jobs[index].arrival, "job ran before arrival");
            executed[index] += segment.end - segment.start;
            busy += segment.end - segment.start;
        }
    }
    require(previous == result.makespan && busy == result.busy_time, "incorrect aggregate time");
    for (std::size_t i = 0; i < jobs.size(); ++i) {
        require(executed[i] == jobs[i].duration, "execution time not conserved");
        const auto& metric = result.jobs[i];
        require(metric.turnaround == metric.completion - jobs[i].arrival &&
                metric.waiting == metric.turnaround - jobs[i].duration && metric.waiting >= 0 &&
                metric.response >= 0 && metric.response <= metric.waiting, "inconsistent job metrics");
    }
}
} // namespace

int main() {
    using namespace scheduling;
    const std::vector<std::pair<const char*, std::function<void()>>> tests{
        {"known RR workload", [] {
            const std::vector<Job> jobs{{"A",0,5,10},{"B",1,2,4},{"C",3,1,6}};
            const auto result = simulate(jobs, Policy::round_robin, 2);
            invariants(jobs, result);
            require(result.jobs[0].completion == 8 && result.jobs[1].completion == 4 &&
                    result.jobs[2].completion == 7 && result.jobs[2].response == 3 &&
                    result.jobs[2].deadline_missed, "incorrect RR timeline or deadline");
        }},
        {"EDF preemption", [] {
            const std::vector<Job> jobs{{"A",0,5,10},{"B",1,2,4},{"C",3,1,6}};
            const auto result = simulate(jobs, Policy::earliest_deadline_first);
            invariants(jobs, result);
            require(result.jobs[0].completion == 8 && result.jobs[1].completion == 3 &&
                    result.jobs[2].completion == 4 && result.jobs[1].response == 0,
                    "urgent job failed to preempt");
        }},
        {"RR boundary arrival before requeue", [] {
            const std::vector<Job> jobs{{"A",0,4,10},{"B",2,1,10}};
            const auto result = simulate(jobs, Policy::round_robin, 2);
            require(result.jobs[1].completion == 3, "arrival at boundary queued after running job");
        }},
        {"idle intervals and unsorted input", [] {
            const std::vector<Job> jobs{{"later",10,1,10},{"first",3,2,5}};
            const auto result = simulate(jobs, Policy::earliest_deadline_first);
            invariants(jobs, result);
            require(!result.timeline.front().job && result.timeline.front().end == 3 &&
                    result.busy_time == 3 && result.makespan == 11 && result.jobs[0].deadline_missed &&
                    !result.jobs[1].deadline_missed, "idle time or deadline equality incorrect");
        }},
        {"stable EDF ties", [] {
            const std::vector<Job> jobs{{"Z",0,2,10},{"A",0,1,10}};
            const auto result = simulate(jobs, Policy::earliest_deadline_first);
            require(result.jobs[0].completion == 2 && result.jobs[1].completion == 3, "tie did not follow input order");
        }},
        {"earlier arrival wins EDF tie", [] {
            const std::vector<Job> jobs{{"A",0,3,10},{"B",1,1,10}};
            const auto result = simulate(jobs, Policy::earliest_deadline_first);
            require(result.jobs[0].completion == 3 && result.jobs[1].completion == 4, "EDF tie preempted older job");
        }},
        {"empty workload", [] {
            const std::vector<Job> jobs;
            const auto result = simulate(jobs, Policy::round_robin);
            std::ostringstream out;
            write_report(out, jobs, result);
            require(result.timeline.empty() && result.makespan == 0 &&
                    out.str().find("cpu_utilization=0.000") != std::string::npos, "empty workload failed");
        }},
        {"CSV CRLF and final line", [] {
            std::istringstream input("id,arrival,duration,deadline\r\nA,0,2,3\r\nB,1,1,3");
            const auto jobs = read_jobs(input);
            require(jobs.size() == 2 && jobs[1].duration == 1, "CSV parsing failed");
        }},
        {"invalid workloads", [] {
            for (const auto& text : {"bad header\n", "id,arrival,duration,deadline\nA,0,0,1",
                    "id,arrival,duration,deadline\nA,-1,1,1", "id,arrival,duration,deadline\nA,0,1,1,2",
                    "id,arrival,duration,deadline\nA,0,1", "id,arrival,duration,deadline\nA,0,1,1\nA,1,1,3",
                    "id,arrival,duration,deadline\nA,0,1000000001,1"}) {
                rejects([&] { std::istringstream input(text); (void)read_jobs(input); });
            }
            const std::vector<Job> jobs{{"A",0,1,2}};
            rejects([&] { (void)simulate(jobs, Policy::round_robin, 0); });
        }},
        {"determinism and execution conservation", [] {
            std::vector<Job> jobs;
            for (int i = 0; i < 25; ++i)
                jobs.push_back({"job" + std::to_string(i), (i * 7) % 19, 1 + i % 5, 10 + (i * 11) % 60});
            for (const auto policy : {Policy::round_robin, Policy::earliest_deadline_first}) {
                const auto first = simulate(jobs, policy, 3);
                const auto second = simulate(jobs, policy, 3);
                invariants(jobs, first);
                std::ostringstream a, b;
                write_report(a, jobs, first);
                write_report(b, jobs, second);
                require(a.str() == b.str(), "identical runs differed");
            }
        }},
        {"write errors", [] {
            const std::vector<Job> jobs{{"A",0,1,2}};
            std::ostringstream output;
            output.setstate(std::ios::badbit);
            rejects([&] { write_report(output, jobs, simulate(jobs, Policy::round_robin)); });
        }}
    };
    int failures = 0;
    for (const auto& test : tests) {
        try { test.second(); std::cout << "PASS " << test.first << '\n'; }
        catch (const std::exception& error) {
            ++failures;
            std::cerr << "FAIL " << test.first << ": " << error.what() << '\n';
        }
    }
    return failures == 0 ? 0 : 1;
}
