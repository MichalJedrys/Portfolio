#ifndef VENDINGMACHINE_HPP
#define VENDINGMACHINE_HPP

#include "Controller.hpp"
#include "Display.hpp"
#include "Feeder.hpp"

class VendingMachine : public Controller, public Display, public Feeder {
public:
    bool work();

private:
    int Option;

    int mOption;

    item product;
};

#endif // VENDINGMACHINE_HPP
