#include "Display.hpp"
#include "ConsoleInput.hpp"

void Display::Start() {
    std::cout << "Vending Machine is starting .";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Display::Shutdown() {
    std::cout << "Vending Machine is shutting down .";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int Display::MainMenu() 
{
    std::cout << " \n";
    std::cout << "Available options:\n";
    std::cout << " 1. Coffee          2.50$ \n";
    std::cout << " 2. Black Tea       1.50$ \n";
    std::cout << " 3. Green Tea       1.50$ \n";
    std::cout << " 4. Cocoa           2.00$ \n";
    std::cout << " 5. Hot Water       1.00$ \n";
    std::cout << " 6. Chicken Soup    3.00$ \n";
    std::cout << " 7. Tomato Soup     3.00$ \n";
    std::cout << " 8. Borscht         3.00$ \n";
    std::cout << " 9. Miso Soup       3.50$ \n";
    std::cout << "10. Onion Soup      3.00$ \n";
    std::cout << "11. Maintenance mode      \n";
    std::cout << " 0. Exit                  \n";
    std::cout << " \n";
    std::cout << "Pick an option: ";
    if (!ReadConsoleInput(Option) || Option < 0 || Option > 11)
    {
        throw IncorrectOption();
    }
    return Option;
}

int Display::MaintenanceMenu(bool requestPassword) {
    if (requestPassword) {
        std::string pass;
        std::cout << "Enter a password: ";
        if (!ReadConsoleInput(pass)) {
            if (std::cin)
                throw IncorrectMTOption();
            return 0;
        }
        if (pass != Controller::password) {
            std::cout << "\nWrong password. Exiting to main menu...\n";
            return 0;
        }
    }
    while (std::cin) {
        std::cout << "\nMaintenance menu\n";
        std::cout << "1. Transaction history\n";
        std::cout << "2. Withdraw collected money\n";
        std::cout << "3. Shut down\n";
        std::cout << "4. Return to main menu\n\n";
        std::cout << "Pick an option: ";
        if (ReadConsoleInput(Option) && Option >= 1 && Option <= 4)
            return Option;
        if (std::cin)
            std::cout << "\nError: " << IncorrectMTOption().what() << '\n';
    }
    return 0;
}

void Display::PrepDisp(item p) {
    std::cout << "\nPreparing " << p.second << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << ".\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << std::endl << p.second << " is ready.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
