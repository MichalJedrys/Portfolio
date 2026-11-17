#include "playruntime/version.hpp"

#include <iostream>
#include <string_view>

int main(int argc, char *argv[]) {
    if (argc == 2 && std::string_view{argv[1]} == "--version") {
        std::cout << "PlayRuntime " << playruntime::version() << '\n';
        return 0;
    }
    if (argc == 1 || (argc == 2 && std::string_view{argv[1]} == "--help")) {
        std::cout << "Usage: playruntime [--help | --version]\n"
                     "Build scaffold only; interaction execution is not implemented.\n";
        return 0;
    }
    std::cerr << "Unsupported arguments. Use --help.\n";
    return 2;
}
