#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <stdint.h>
#include <windows.h>
#include <iostream>
#include <conio.h>

class Console
{
public:
    /*
    * @brief Move cursor to the top-left corner.
    */
    void clear() noexcept;

    /*
    * @brief Print menu options.
    */
    void menu(uint16_t score) noexcept;

    /*
    * @brief Menu input management.
    */
    bool inputMenu() noexcept;

    /*
    * @brief Get status.
    */
    bool getStatus() noexcept;

    /*
    * @brief Check if player wants to close the app.
    */
    bool exitGame() noexcept;

private:
    // Check whether if player wants to close the app.
    bool exit = false;
    // Check whether if player wants resume playing.
    bool resume = false;
};

#endif  // CONSOLE_HPP