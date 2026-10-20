#include "Feeder.hpp"
#include "ConsoleInput.hpp"

void Feeder::SoySauce() 
{
    char s;
    std::cout << "\nDo you want to add soy sauce? (y/n): ";
    if (!ReadConsoleInput(s) || (s != 'y' && s != 'n')) {
        throw InvalidOperator();
    }
}

void Feeder::MilkAndSugar(int ID) {
    int sugar;
    char m;
    if (ID == 1) 
    {
        std::cout << "\nDo you want to add milk? (y/n): ";
        if (!ReadConsoleInput(m) || (m != 'y' && m != 'n')) {
            throw InvalidOperator();
        }
    }
    if (ID >= 1 && ID < 5) {
        std::cout << "\nSet amount of sugar you want (0-6): ";
        if (!ReadConsoleInput(sugar) || sugar < 0 || sugar > 6) {
            throw InvalidOperator();
        }
    }
}
