#include "srt/parser.hpp"

#include <charconv>
#include <istream>
#include <map>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string_view>
#include <system_error>

namespace srt {
namespace {

std::string_view trim(std::string_view value) {
    const auto first = value.find_first_not_of(" \t");
    if (first == std::string_view::npos) return {};
    const auto last = value.find_last_not_of(" \t");
    return value.substr(first, last - first + 1);
}

std::int64_t integer(std::string_view value) {
    if (value.empty() || value.find_first_not_of("0123456789") != std::string_view::npos)
        throw std::runtime_error("expected an unsigned decimal integer");
    std::int64_t result = 0;
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size())
        throw std::runtime_error("integer is out of range");
    return result;
}

std::int64_t timestamp(std::string_view value) {
    if (value.size() != 12 || value[2] != ':' || value[5] != ':' || value[8] != ',')
        throw std::runtime_error("expected timestamp HH:MM:SS,mmm");
    const auto hours = integer(value.substr(0, 2));
    const auto minutes = integer(value.substr(3, 2));
    const auto seconds = integer(value.substr(6, 2));
    const auto millis = integer(value.substr(9, 3));
    if (minutes > 59 || seconds > 59)
        throw std::runtime_error("timestamp minutes and seconds must be below 60");
    return ((hours * 60 + minutes) * 60 + seconds) * 1000 + millis;
}

std::string escape(std::string_view value) {
    std::string result;
    for (const char c : value) {
        switch (c) {
        case '\n': result += "\\n"; break;
        case '\t': result += "\\t"; break;
        case '\\': result += "\\\\"; break;
        case '\'': result += "\\'"; break;
        default: result += c; break;
        }
    }
    return result;
}

} // namespace

std::vector<Cue> parse(std::istream& input) {
    std::vector<Cue> cues;
    std::string line;
    std::size_t line_number = 0;
    auto read_line = [&]() {
        if (!std::getline(input, line)) {
            if (!input.eof() || input.bad()) throw std::runtime_error("failed to read subtitle input");
            return false;
        }
        ++line_number;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line_number == 1 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line.erase(0, 3);
        return true;
    };

    try {
        while (read_line()) {
            if (trim(line).empty()) continue;
            Cue cue{};
            cue.number = integer(trim(line));
            if (cue.number == 0 || (!cues.empty() && cue.number <= cues.back().number))
                throw std::runtime_error("cue numbers must be positive and strictly increasing");
            if (!read_line()) throw std::runtime_error("missing timing line");
            const auto arrow = line.find("-->");
            if (arrow == std::string::npos) throw std::runtime_error("missing --> separator");
            cue.start_ms = timestamp(trim(std::string_view(line).substr(0, arrow)));
            cue.end_ms = timestamp(trim(std::string_view(line).substr(arrow + 3)));
            if (cue.start_ms >= cue.end_ms) throw std::runtime_error("cue end must be after start");
            while (read_line() && !trim(line).empty()) {
                if (!cue.text.empty()) cue.text += '\n';
                cue.text += line;
            }
            if (cue.text.empty()) throw std::runtime_error("cue must contain subtitle text");
            cues.push_back(std::move(cue));
        }
    } catch (const std::runtime_error& error) {
        throw std::runtime_error("line " + std::to_string(line_number) + ": " + error.what());
    }
    return cues;
}

std::vector<Change> timeline(const std::vector<Cue>& cues) {
    struct Event { std::size_t cue; bool starts; };
    std::map<std::int64_t, std::vector<Event>> events;
    for (std::size_t i = 0; i < cues.size(); ++i) {
        const auto& cue = cues[i];
        if (cue.start_ms < 0 || cue.end_ms <= cue.start_ms || cue.text.empty())
            throw std::runtime_error("invalid cue interval or empty text");
        events[cue.start_ms].push_back({i, true});
        events[cue.end_ms].push_back({i, false});
    }
    std::set<std::size_t> active;
    std::vector<Change> changes;
    std::string previous;
    for (const auto& entry : events) {
        for (const auto& event : entry.second) {
            if (event.starts) active.insert(event.cue);
            else active.erase(event.cue);
        }
        std::string text;
        for (const auto index : active) {
            if (!text.empty()) text += '\n';
            text += cues[index].text;
        }
        if (text != previous) {
            changes.push_back({entry.first, text});
            previous = std::move(text);
        }
    }
    return changes;
}

void write_timeline(std::ostream& output, const std::vector<Change>& changes) {
    for (const auto& change : changes) {
        if (change.time_ms < 0) throw std::runtime_error("negative timeline timestamp");
        const auto fraction = std::to_string(1000 + change.time_ms % 1000).substr(1);
        output << "at " << std::to_string(change.time_ms / 1000) << '.' << fraction
               << " show '" << escape(change.text) << "'\n";
    }
    if (!output) throw std::runtime_error("failed to write timeline output");
}

} // namespace srt
