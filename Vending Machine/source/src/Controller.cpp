#include "Controller.hpp"
#include "ConsoleInput.hpp"

bool Controller::PaymentProcessing(item p) 
{
    std::cout << "\nSelected: " << p.second << " (" << p.first << "$)\n";
    std::cout << "Accepted coins: 0.25$, 0.5$, 1$, 2$, 5$. Change is returned.\n";
    std::cout << "Enter 0 to cancel payment and return inserted coins.\n";
    double Rest = p.first;
    double Payment = 0;
    const auto cancel = [&]() {
        std::cout << "\nPayment cancelled.\n";
        if (Rest < p.first)
            std::cout << "Refunding " << p.first - Rest << "$\n";
        return false;
    };

    while (Rest > 0) {
        std::cout << "\nRemaining: " << Rest << "$\n";
        std::cout << "Insert coin: ";
        if (!ReadConsoleInput(Payment)) {
            if (!std::cin)
                return cancel();
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
    std::string c;
    switch (mOption) 
    {
    case 1:
        std::cout << "\nTransaction history:\n";
        if (PaymentsVec.empty())
            std::cout << "No transactions yet.\n";
        for (auto k : PaymentsVec)
        {
            std::cout << k.first << " - " << k.second << std::endl;
        }
        std::cout << "\nReturning to maintenance menu...\n";
        std::cout << "Press Enter to continue.\n";
        std::getline(std::cin, c);
        break;
    case 2:
        std::cout << "\nCollected: " << MoneySum << "$\n";
        std::cout << "Returning to maintenance menu...\n";
        std::cout << "Press Enter to continue.\n";
        std::getline(std::cin, c);
        MoneySum = 0;
        break;
    case 4:
        break;
    }
}

item Controller::SelectBev(int Option)
{
    return Items[Option - 1];
}
