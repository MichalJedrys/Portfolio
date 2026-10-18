#include "srt/parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: srt-parser INPUT.srt [OUTPUT.txt]\n"
                     "Without OUTPUT.txt, writes the timeline to standard output.\n";
        return 0;
    }
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: srt-parser INPUT.srt [OUTPUT.txt]\n";
        return 2;
    }
    try {
        const std::filesystem::path input_path(argv[1]);
        if (argc == 3) {
            const std::filesystem::path output_path(argv[2]);
            if (std::filesystem::exists(output_path) && std::filesystem::equivalent(input_path, output_path))
                throw std::runtime_error("input and output must be different files");
        }
        std::ifstream input(input_path, std::ios::binary);
        if (!input) throw std::runtime_error("cannot open input file");
        const auto changes = srt::timeline(srt::parse(input));
        input.close();
        if (argc == 3) {
            std::ofstream output(argv[2], std::ios::binary);
            if (!output) throw std::runtime_error("cannot open output file");
            srt::write_timeline(output, changes);
            output.close();
            if (!output) throw std::runtime_error("failed to close output file");
        } else {
            srt::write_timeline(std::cout, changes);
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "srt-parser: " << error.what() << '\n';
        return 1;
    }
}
