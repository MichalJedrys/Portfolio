#include "Table.hpp"
#include "Menu.hpp"
#include <fstream>

void log(const std::string& message) {
    std::ofstream logFile("log.txt", std::ios_base::app);
    logFile << message << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: LibraryManagementSystem [database-path]\n"
                  << "Without arguments, open or create LibraryDatabase.db.\n";
        return 0;
    }
    if (argc > 2 || (argc == 2 && (argv[1][0] == '\0' || std::string(argv[1]).rfind("--", 0) == 0))) {
        std::cerr << "Usage: LibraryManagementSystem [database-path]\n";
        return 2;
    }
    const std::string databasePath = argc == 2 ? argv[1] : "LibraryDatabase.db";
    database* db = nullptr;
    int rc = sqlite3_open(databasePath.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database '" << databasePath << "': " << errorMessage(db) << std::endl;
        if (db)
            sqlite3_close(db);
        return 1;
    }

    if (!BooksTable::createBooksTable(db)) {
        std::cerr << "Failed to create books table." << std::endl;
        sqlite3_close(db);
        return 1;
    }

    displayMenu(db);

    sqlite3_close(db);
    return 0;
}