#include "VendingMachine.hpp"

bool VendingMachine::work()
{
    Option = -1;
    mOption = -1;
    try
    {
        Option = MainMenu();
    }
    catch (const IncorrectOption& ex)
    {
        if (std::cin)
            std::cout << "\nError: " << ex.what() << std::endl;
        return static_cast<bool>(std::cin);
    }
    
    if (Option == 0) {
        Shutdown();
        return false;
    }
    if (Option == 11) {
        do {
            try {
                mOption = MaintenanceMenu(mOption == -1);
            }
            catch (const IncorrectMTOption& ex) {
                if (std::cin)
                    std::cout << "\nError: " << ex.what() << std::endl;
                return static_cast<bool>(std::cin);
            }
            if (mOption == 3) {
                Shutdown();
                return false;
            }
            SelectMOption(mOption);
        } while (std::cin && (mOption == 1 || mOption == 2));
    }
    else{
        product = SelectBev(Option);
        try
        {
            if (Option <= 5)
                MilkAndSugar(Option);
            else
                SoySauce();
        }
        catch (const InvalidOperator& ex)
        {
            if (std::cin)
                std::cout << "\nError: " << ex.what() << std::endl;
            return static_cast<bool>(std::cin);
        }
        try
        {
            if (PaymentProcessing(product))
                PrepDisp(product);
        }
        catch (const InvalidPayment& ex)
        {
            std::cout << "\nError: " << ex.what() << std::endl;
            return static_cast<bool>(std::cin);
        }
    }
    return static_cast<bool>(std::cin);
}
