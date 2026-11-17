#include "playruntime/runtime.hpp"
#include "playruntime/version.hpp"

#include <array>
#include <iostream>
#include <string_view>

namespace {
int run_demo() {
    using namespace playruntime;
    const std::array rules{LightRule{EventKind::button, LightColour::cyan}};
    Runtime runtime{rules};
    const std::array events{Event{0, 0, ButtonPressed{}}, Event{500000, 1, MotionDetected{}},
                            Event{1000000, 2, ButtonPressed{}}};
    if (runtime.submit_batch(events)) {
        std::cerr << "Demo input rejected.\n";
        return 1;
    }
    const auto result = runtime.advance_to(1000000);
    if (result.error) {
        std::cerr << "Demo advancement failed.\n";
        return 1;
    }
    for (const auto &record : result.commands) {
        // This example's only configured command is SetLight(cyan).
        std::cout << record.timestamp << "us event=" << record.source_sequence << " light=cyan\n";
    }
    return 0;
}
} // namespace

int main(int argc, char *argv[]) {
    if (argc == 2 && std::string_view{argv[1]} == "--version") {
        std::cout << "PlayRuntime " << playruntime::version() << '\n';
        return 0;
    }
    if (argc == 2 && std::string_view{argv[1]} == "--demo") {
        return run_demo();
    }
    if (argc == 1 || (argc == 2 && std::string_view{argv[1]} == "--help")) {
        std::cout << "Usage: playruntime [--help | --version | --demo]\n"
                     "--demo runs a programmatic button-to-light example.\n"
                     "Timers, state machines and scripting are not implemented.\n";
        return 0;
    }
    std::cerr << "Unsupported arguments. Use --help.\n";
    return 2;
}
