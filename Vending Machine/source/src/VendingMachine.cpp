#include "VendingMachine.hpp"

void VendingMachine::work()
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
        return;
    }
    
    if (Option == 11) {
        try
        {
            mOption = MaintenanceMenu();
        }
        catch (const IncorrectMTOption& ex)
        {
            if (std::cin)
                std::cout << "\nError: " << ex.what() << std::endl;
            return;
        }
        if (mOption == 0) {
            return;
        }
        else {
            SelectMOption(mOption);
            return;
        }
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
            return;
        }
        try
        {
            if (PaymentProcessing(product))
                PrepDisp(product);
        }
        catch (const InvalidPayment& ex)
        {
            std::cout << "\nError: " << ex.what() << std::endl;
            return;
        }
    }
}
