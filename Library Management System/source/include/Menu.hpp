#ifndef MENU_HPP
#define MENU_HPP

#include <sqlite3.h>
#include "SQLiteWrapper.hpp"
#include <iostream>

void displayMenu(database* db, std::istream& input = std::cin, std::ostream& output = std::cout);

#endif // MENU_HPP