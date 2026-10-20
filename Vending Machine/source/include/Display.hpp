#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Exceptions.hpp"
#include "Controller.hpp"
#include <iostream>
#include <chrono>
#include <thread>

class Display {
public:
    void Start();

    void Shutdown();

    int MainMenu();

    int MaintenanceMenu(bool requestPassword = true);

    void PrepDisp(item p);

private:
    int Option;
};

#endif // DISPLAY_HPP
