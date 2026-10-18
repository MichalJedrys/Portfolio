#include "scheduling/scheduler.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <deque>
#include <iomanip>
#include <istream>
#include <locale>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <tuple>
#include <utility>

namespace scheduling {
namespace {
constexpr Time max_input_time = 1'000'000'000;
constexpr std::size_t max_jobs = 10'000;
constexpr std::size_t max_steps = 1'000'000;

Time number(std::string_view value) {
    if (value.empty() || value.find_first_not_of("0123456789") != std::string_view::npos)
        throw std::runtime_error("times must be unsigned decimal integers");
    Time result = 0;
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size() || result > max_input_time)
        throw std::runtime_error("time is out of range (maximum 1000000000)");
    return result;
}

void validate(std::span<const Job> jobs) {
    if (jobs.size() > max_jobs) throw std::runtime_error("maximum 10000 jobs exceeded");
    std::set<std::string> ids;
    for (const auto& job : jobs) {
        if (job.id.empty() || job.id.find_first_not_of(
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-") != std::string::npos)
            throw std::runtime_error("job IDs must use ASCII letters, digits, underscores, or hyphens");
        if (!ids.insert(job.id).second) throw std::runtime_error("job IDs must be unique");
        if (job.arrival < 0 || job.arrival > max_input_time || job.duration <= 0 ||
            job.duration > max_input_time || job.deadline < 0 || job.deadline > max_input_time)
            throw std::runtime_error("arrival/deadline must be in [0,1000000000] and duration in [1,1000000000]");
    }
}
} // namespace

std::vector<Job> read_jobs(std::istream& input) {
    std::string line;
    auto read_line = [&]() {
        if (!std::getline(input, line)) {
            if (!input.eof() || input.bad()) throw std::runtime_error("failed to read workload");
            return false;
        }
        if (!line.empty() && line.back() == '\r') line.pop_back();
        return true;
    };
    if (!read_line() || line != "id,arrival,duration,deadline")
        throw std::runtime_error("expected header id,arrival,duration,deadline");
    std::vector<Job> jobs;
    std::size_t line_number = 1;
    while (read_line()) {
        ++line_number;
        try {
            std::array<std::string_view, 4> fields{};
            std::string_view remaining(line);
            for (std::size_t i = 0; i < fields.size(); ++i) {
                const auto comma = remaining.find(',');
                if (i + 1 == fields.size()) {
                    if (comma != std::string_view::npos) throw std::runtime_error("too many fields");
                    fields[i] = remaining;
                } else {
                    if (comma == std::string_view::npos) throw std::runtime_error("too few fields");
                    fields[i] = remaining.substr(0, comma);
                    remaining.remove_prefix(comma + 1);
                }
            }
            jobs.push_back({std::string(fields[0]), number(fields[1]), number(fields[2]), number(fields[3])});
            if (jobs.size() > max_jobs) throw std::runtime_error("maximum 10000 jobs exceeded");
        } catch (const std::runtime_error& error) {
            throw std::runtime_error("line " + std::to_string(line_number) + ": " + error.what());
        }
    }
    validate(jobs);
    return jobs;
}

Result simulate(std::span<const Job> jobs, Policy policy, Time quantum) {
    validate(jobs);
    if (policy != Policy::round_robin && policy != Policy::earliest_deadline_first)
        throw std::runtime_error("unknown scheduling policy");
    if (quantum <= 0 || quantum > max_input_time) throw std::runtime_error("quantum must be in [1,1000000000]");
    Result result;
    result.jobs.resize(jobs.size());
    std::vector<Time> remaining;
    std::vector<std::optional<Time>> first_start(jobs.size());
    for (const auto& job : jobs) remaining.push_back(job.duration);
    std::vector<std::size_t> arrivals(jobs.size());
    std::iota(arrivals.begin(), arrivals.end(), 0);
    std::stable_sort(arrivals.begin(), arrivals.end(), [&](auto a, auto b) {
        return jobs[a].arrival < jobs[b].arrival;
    });
    std::deque<std::size_t> ready;
    Time now = 0;
    std::size_t next = 0;
    std::size_t finished = 0;
    std::size_t steps = 0;
    auto enqueue = [&]() {
        while (next < arrivals.size() && jobs[arrivals[next]].arrival <= now)
            ready.push_back(arrivals[next++]);
    };
    auto segment = [&](Time start, Time end, std::optional<std::size_t> job) {
        if (!result.timeline.empty() && result.timeline.back().end == start && result.timeline.back().job == job)
            result.timeline.back().end = end;
        else result.timeline.push_back({start, end, job});
    };
    while (finished < jobs.size()) {
        if (++steps > max_steps) throw std::runtime_error("simulation exceeded 1000000 scheduling steps; increase RR quantum or reduce workload");
        enqueue();
        if (ready.empty()) {
            const auto arrival = jobs[arrivals[next]].arrival;
            segment(now, arrival, std::nullopt);
            now = arrival;
            enqueue();
        }
        auto chosen = ready.begin();
        if (policy == Policy::earliest_deadline_first) {
            chosen = std::min_element(ready.begin(), ready.end(), [&](auto a, auto b) {
                return std::tie(jobs[a].deadline, jobs[a].arrival, a) <
                       std::tie(jobs[b].deadline, jobs[b].arrival, b);
            });
        }
        const auto index = *chosen;
        ready.erase(chosen);
        if (!first_start[index]) first_start[index] = now;
        Time duration = remaining[index];
        if (policy == Policy::round_robin) duration = std::min(duration, quantum);
        else if (next < arrivals.size()) duration = std::min(duration, jobs[arrivals[next]].arrival - now);
        segment(now, now + duration, index);
        now += duration;
        result.busy_time += duration;
        remaining[index] -= duration;
        enqueue(); // Arrivals at the quantum boundary enter before the preempted RR job.
        if (remaining[index] == 0) {
            const auto turnaround = now - jobs[index].arrival;
            result.jobs[index] = {now, turnaround, turnaround - jobs[index].duration,
                                  *first_start[index] - jobs[index].arrival, now > jobs[index].deadline};
            ++finished;
        } else ready.push_back(index);
    }
    result.makespan = now;
    return result;
}

void write_report(std::ostream& output, std::span<const Job> jobs, const Result& result) {
    if (result.jobs.size() != jobs.size()) throw std::runtime_error("job/metric count mismatch");
    std::ostringstream report;
    report.imbue(std::locale::classic());
    report << "timeline\n";
    for (const auto& segment : result.timeline) {
        if (segment.job && *segment.job >= jobs.size()) throw std::runtime_error("invalid timeline job index");
        report << segment.start << '-' << segment.end << ' '
               << (segment.job ? jobs[*segment.job].id : "<idle>") << '\n';
    }
    report << "\nid,completion,turnaround,waiting,response,deadline_missed\n";
    Time total_waiting = 0;
    Time total_turnaround = 0;
    Time total_response = 0;
    std::size_t missed = 0;
    for (std::size_t i = 0; i < jobs.size(); ++i) {
        const auto& metric = result.jobs[i];
        report << jobs[i].id << ',' << metric.completion << ',' << metric.turnaround << ','
               << metric.waiting << ',' << metric.response << ',' << (metric.deadline_missed ? "yes" : "no") << '\n';
        total_waiting += metric.waiting;
        total_turnaround += metric.turnaround;
        total_response += metric.response;
        if (metric.deadline_missed) ++missed;
    }
    const auto count = static_cast<double>(jobs.size());
    const auto makespan = static_cast<double>(result.makespan);
    report << std::fixed << std::setprecision(3)
           << "\nmean_waiting=" << (count ? static_cast<double>(total_waiting) / count : 0.0)
           << "\nmean_turnaround=" << (count ? static_cast<double>(total_turnaround) / count : 0.0)
           << "\nmean_response=" << (count ? static_cast<double>(total_response) / count : 0.0)
           << "\ndeadline_misses=" << missed
           << "\ncpu_utilization=" << (makespan ? static_cast<double>(result.busy_time) / makespan : 0.0)
           << "\nthroughput=" << (makespan ? count / makespan : 0.0) << '\n';
    output << report.str();
    if (!output) throw std::runtime_error("failed to write report");
}

} // namespace scheduling
