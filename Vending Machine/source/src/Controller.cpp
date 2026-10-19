#include "Controller.hpp"
#include <limits>

bool Controller::PaymentProcessing(item p) 
{
    std::cout << "\n  Vending machine accepts only:  \n"; 
    std::cout << "  0.25$, 0.5$, 1$, 2$, 5$ coins. \n"; 
    std::cout << "  The machine gives the change.  \n\n"; 
    double Rest = p.first;
    double Payment = 0;
    const auto cancel = [&]() {
        std::cout << "\nPayment cancelled.\n";
        if (Rest < p.first)
            std::cout << "Refunding " << p.first - Rest << "$\n";
        return false;
    };

    while (Rest > 0) {
        std::cout << "(Enter 0 to cancel payment and return inserted coins)\n";
        std::cout << " Insert coin : ";
        if (!(std::cin >> Payment)) {
            if (std::cin.eof() || std::cin.bad())
                return cancel();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nError: " << InvalidPayment().what() << '\n';
            continue;
        }
        if (Payment != 0 && Payment != 0.25 && Payment != 0.5 && Payment != 1 && Payment != 2 && Payment != 5) 
        {
            std::cout << "\nError: " << InvalidPayment().what() << '\n';
            continue;
        }
        if (Payment == 0) {
            return cancel();
        }
        Rest = Rest - Payment;
    }

    MoneySum = MoneySum + p.first;
    PaymentsVec.push_back(p);

    if (Rest < 0) {
        std::cout << "\nGiving change .";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << ".\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "\nYour change is " << -Rest << "$\n";
    }
    return true;
}

void Controller::SelectMOption(int mOption) 
{
    char c;
    switch (mOption) 
    {
    case 1:
        for (auto k : PaymentsVec)
        {
            std::cout << k.first << " - " << k.second << std::endl;
        }
        std::cout << "\nReturning to maintenance menu...\n";
        std::cout << "Press any key to continue.\n";
        std::cin >> c;
        break;
    case 2:
        std::cout << "\nCollected: " << MoneySum << "$\n";
        std::cout << "Returning to maintenance menu...\n";
        std::cout << "Press any key to continue.\n";
        std::cin >> c;
        MoneySum = 0;
        break;
    case 3:
        std::cout << "Vending Machine is shutting down .";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << ".";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << ".\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        exit(0);
        break;
    case 4:
        break;
    }
}

item Controller::SelectBev(int Option)
{
    return Items[Option - 1];
}
