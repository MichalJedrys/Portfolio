#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace srt {

struct Cue {
    std::int64_t number;
    std::int64_t start_ms;
    std::int64_t end_ms;
    std::string text;
};

struct Change {
    std::int64_t time_ms;
    std::string text;
};

// Throws std::runtime_error for malformed input or failed input/output.
std::vector<Cue> parse(std::istream& input);
// Half-open intervals [start_ms, end_ms); overlapping text follows input order.
std::vector<Change> timeline(const std::vector<Cue>& cues);
void write_timeline(std::ostream& output, const std::vector<Change>& changes);

} // namespace srt
