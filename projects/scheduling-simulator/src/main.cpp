#include "scheduling/scheduler.hpp"

#include <charconv>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

int main(int argc, char* argv[]) {
    const auto usage = "Usage: scheduling-simulator rr JOBS.csv [QUANTUM]\n"
                       "       scheduling-simulator edf JOBS.csv\n";
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        std::cout << usage;
        return 0;
    }
    if (argc < 3 || argc > 4) { std::cerr << usage; return 2; }
    try {
        const std::string_view name(argv[1]);
        scheduling::Policy policy;
        if (name == "rr") policy = scheduling::Policy::round_robin;
        else if (name == "edf" && argc == 3) policy = scheduling::Policy::earliest_deadline_first;
        else throw std::runtime_error("use rr with an optional quantum, or edf without a quantum");
        scheduling::Time quantum = 2;
        if (argc == 4) {
            const std::string_view value(argv[3]);
            const auto parsed = std::from_chars(value.data(), value.data() + value.size(), quantum);
            if (value.empty() || value.find_first_not_of("0123456789") != std::string_view::npos ||
                parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size())
                throw std::runtime_error("quantum must be a positive decimal integer");
        }
        std::ifstream input(argv[2]);
        if (!input) throw std::runtime_error("cannot open workload file");
        const auto jobs = scheduling::read_jobs(input);
        scheduling::write_report(std::cout, jobs, scheduling::simulate(jobs, policy, quantum));
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "scheduling-simulator: " << error.what() << '\n';
        return 1;
    }
}
