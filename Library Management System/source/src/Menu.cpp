#include "Menu.hpp"
#include "Table.hpp"
#include <iostream>
#include <sstream>

namespace {
bool readPositiveInteger(std::istream& input, std::ostream& output, int& value) {
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream number(line);
        if ((number >> value) && (number >> std::ws).eof() && value > 0)
            return true;
        output << "Please enter a positive whole number: ";
    }
    return false;
}

bool readField(std::istream& input, std::ostream& output, const char* prompt,
               std::string& value, bool required = true) {
    while (true) {
        output << prompt;
        if (!std::getline(input, value))
            return false;
        if (!required || value.find_first_not_of(" \t\r") != std::string::npos)
            return true;
        output << "This field cannot be blank.\n";
    }
}
}

void displayMenu(database* db, std::istream& input, std::ostream& output) {
    int choice = 0;
    std::vector<Book> books;
    do {
        output << "\nWelcome to the library!" << std::endl;
        output << "1. Add Book" << std::endl;
        output << "2. View Books" << std::endl;
        output << "3. Update Book" << std::endl;
        output << "4. Delete Book" << std::endl;
        output << "5. Exit" << std::endl;
        output << "Enter your choice: ";
        if (!readPositiveInteger(input, output, choice))
            return;

        switch (choice) {
            case 1: {
                Book book;
                if (!readField(input, output, "Enter title: ", book.mTitle) ||
                    !readField(input, output, "Enter author: ", book.mAuthor) ||
                    !readField(input, output, "Enter genre: ", book.mGenre, false) ||
                    !readField(input, output, "Enter ISBN: ", book.mIsbn))
                    return;

                if (BooksTable::addBook(db, book)) {
                    output << "Book added successfully." << std::endl;
                } else {
                    output << "Failed to add book." << std::endl;
                }
                break;
            }
            case 2:
                books = BooksTable::viewBooks(db);
                for(auto &book : books)
                {
                    output << book << std::endl;
                }
                break;
            case 3: {
                Book book;
                output << "Enter book ID to update: ";
                if (!readPositiveInteger(input, output, book.mId) ||
                    !readField(input, output, "Enter new title: ", book.mTitle) ||
                    !readField(input, output, "Enter new author: ", book.mAuthor) ||
                    !readField(input, output, "Enter new genre: ", book.mGenre, false) ||
                    !readField(input, output, "Enter new ISBN: ", book.mIsbn))
                    return;

                if (BooksTable::updateBook(db, book)) {
                    output << "Book updated successfully." << std::endl;
                } else {
                    output << "Failed to update book." << std::endl;
                }
                break;
            }
            case 4: {
                int bookId;
                output << "Enter book ID to delete: ";
                if (!readPositiveInteger(input, output, bookId))
                    return;

                if (BooksTable::deleteBook(db, bookId)) {
                    output << "Book deleted successfully." << std::endl;
                } else {
                    output << "Failed to delete book." << std::endl;
                }
                break;
            }
            case 5:
                output << "Exiting..." << std::endl;
                break;
            default:
                output << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 5);
}