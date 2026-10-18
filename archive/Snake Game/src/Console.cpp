#include "Console.hpp"

void Console::clear() noexcept {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the cursor position to the top-left corner
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsole, cursorPosition);
}


void Console::menu(uint16_t score) noexcept {
    system("cls");
    std::cout << "GAME OVER" << std::endl;
    std::cout << "YOUR SCORE: " << score << "!" << std::endl;
    std::cout << "Do you want to play again? Y/N" << std::endl;
    while(!inputMenu())
    {
        Sleep(1);
    }
}

bool Console::inputMenu() noexcept  {
    if (_kbhit()) {
        switch (_getch()) {
        case 'y':
            resume = true;
            return true;
        case 'n':
            exit = true;
            return true;
        }
    }
    return false;
}

bool Console::getStatus() noexcept{
    return resume;
}

bool Console::exitGame() noexcept 
{
    return exit;
}

