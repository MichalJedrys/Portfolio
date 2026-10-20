#ifndef CONSOLE_INPUT_HPP
#define CONSOLE_INPUT_HPP

#include <iostream>
#include <sstream>
#include <string>

template <typename T>
bool ReadConsoleInput(T& value) {
    std::string line;
    if (!std::getline(std::cin, line))
        return false;
    std::istringstream input(line);
    T parsed{};
    if (!(input >> parsed))
        return false;
    input >> std::ws;
    if (!input.eof())
        return false;
    value = parsed;
    return true;
}

#endif // CONSOLE_INPUT_HPP
