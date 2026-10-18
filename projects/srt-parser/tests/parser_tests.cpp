#include "srt/parser.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
void rejects(const std::string& input) {
    std::istringstream stream(input);
    try { (void)srt::parse(stream); }
    catch (const std::runtime_error&) { return; }
    throw std::runtime_error("malformed input was accepted");
}
std::vector<srt::Change> convert(const std::string& input) {
    std::istringstream stream(input);
    return srt::timeline(srt::parse(stream));
}
} // namespace

int main() {
    const std::vector<std::pair<const char*, std::function<void()>>> tests{
        {"empty input", [] { require(convert("").empty(), "empty input should have no changes"); }},
        {"EOF without blank line", [] {
            const auto changes = convert("1\n00:00:00,001 --> 00:00:00,009\nFinal line");
            require(changes.size() == 2 && changes[0].time_ms == 1 && changes[1].time_ms == 9,
                    "millisecond precision or final cue lost");
            require(changes[0].text == "Final line" && changes[1].text.empty(), "incorrect final text");
        }},
        {"BOM CRLF multiline", [] {
            const auto changes = convert("\xEF\xBB\xBF" "1\r\n00:00:00,000 --> 00:00:01,000\r\nA\r\nB\r\n");
            require(changes.size() == 2 && changes[0].text == "A\nB", "BOM or CRLF mishandled");
        }},
        {"overlap nesting and input order", [] {
            const auto changes = convert("1\n00:00:01,000 --> 00:00:04,000\nA\n\n"
                                         "2\n00:00:02,000 --> 00:00:03,000\nB\n\n"
                                         "3\n00:00:00,500 --> 00:00:02,500\nC\n");
            require(changes.size() == 6, "unexpected change count");
            require(changes[0].text == "C" && changes[1].text == "A\nC" &&
                    changes[2].text == "A\nB\nC" && changes[3].text == "A\nB" &&
                    changes[4].text == "A" && changes[5].text.empty(), "incorrect overlap order");
        }},
        {"simultaneous boundaries", [] {
            const auto changes = convert("1\n00:00:01,000 --> 00:00:02,000\nA\n\n"
                                         "2\n00:00:02,000 --> 00:00:03,000\nB");
            require(changes.size() == 3 && changes[1].time_ms == 2000 && changes[1].text == "B",
                    "transient empty text at shared boundary");
        }},
        {"identical text transitions", [] {
            const auto changes = convert("1\n00:00:01,000 --> 00:00:02,000\nA\n\n"
                                         "2\n00:00:02,000 --> 00:00:03,000\nA");
            require(changes.size() == 2 && changes[1].time_ms == 3000, "redundant text change");
        }},
        {"bad timestamps", [] {
            rejects("1\n00:60:00,000 --> 01:00:01,000\nA");
            rejects("1\n00:00:AA,000 --> 00:00:01,000\nA");
            rejects("1\n00:00:00.000 --> 00:00:01,000\nA");
            rejects("1\n00:00:02,000 --> 00:00:01,000\nA");
            rejects("1\n00:00:01,000 --> 00:00:01,000\nA");
        }},
        {"bad records", [] {
            rejects("0\n00:00:00,000 --> 00:00:01,000\nA");
            rejects("999999999999999999999999999\n00:00:00,000 --> 00:00:01,000\nA");
            rejects("1\n00:00:00,000 --> 00:00:01,000\n\n");
            rejects("1\n");
            rejects("1\n00:00:00,000 --> 00:00:01,000\nA\n\n1\n00:00:02,000 --> 00:00:03,000\nB");
        }},
        {"parse calls are independent", [] {
            std::istringstream input("1\n00:00:00,000 --> 00:00:01,000\nA");
            const auto cues = srt::parse(input);
            require(srt::timeline(cues).size() == 2 && srt::timeline(cues).size() == 2, "state leaked");
        }},
        {"output escaping", [] {
            std::ostringstream output;
            srt::write_timeline(output, {{1005, "It's\na\\b"}, {2000, ""}});
            require(output.str() == "at 1.005 show 'It\\'s\\na\\\\b'\nat 2.000 show ''\n", "incorrect output");
        }},
        {"output errors", [] {
            std::ostringstream output;
            output.setstate(std::ios::badbit);
            try { srt::write_timeline(output, {{1, "A"}}); }
            catch (const std::runtime_error&) { return; }
            throw std::runtime_error("write failure ignored");
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
