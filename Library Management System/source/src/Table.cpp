#include "Table.hpp"
#include "Book.hpp"
#include <iostream>

bool BooksTable::createBooksTable(database* db) {
    std::string sqlCreateTable =
        "CREATE TABLE IF NOT EXISTS books ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "author TEXT NOT NULL,"
        "genre TEXT,"
        "isbn TEXT UNIQUE NOT NULL);";
    
    return prepAndEvaluate(db, sqlCreateTable);
}

bool BooksTable::clearBooksTable(database* db)
{
    std::string sqlDeleteTable = "DELETE FROM books";
    prepAndEvaluate(db, sqlDeleteTable);

    std::string sqlVacummTable ="VACUUM";
    return prepAndEvaluate(db, sqlDeleteTable);
}

bool BooksTable::addBook(database* db, const Book& book) {
    std::string sql = "INSERT INTO books (title, author, genre, isbn) VALUES (?, ?, ?, ?)";
    sqlStatement* statement;
    if (!prepareStatement(db, sql, &statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        return false;
    }
    setText(statement, 1, book.mTitle);
    setText(statement, 2, book.mAuthor);
    setText(statement, 3, book.mGenre);
    setText(statement, 4, book.mIsbn);
    if (!evaluateStatement(statement)) {
        if (sqlite3_extended_errcode(db) == SQLITE_CONSTRAINT_UNIQUE)
            std::cerr << "A book with this ISBN already exists." << std::endl;
        else
            std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        sqlFinalize(statement);
        return false;
    }
    sqlFinalize(statement);
    return true;
}

std::vector<Book> BooksTable::viewBooks(database* db) {
    std::vector<Book> books;
    std::string sql = "SELECT id, title, author, genre, isbn FROM books";
    sqlStatement* statement;
    if (!prepareStatement(db, sql, &statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        return books;
    }
    Book book;
    while (hasRow(statement)) {
        book.mId = getInt(statement, 0);
        book.mTitle = getText(statement, 1);
        book.mAuthor = getText(statement, 2);
        book.mGenre = getText(statement, 3);
        book.mIsbn = getText(statement, 4);
        books.push_back(book);
    }
    sqlFinalize(statement);
    return books;
}

bool BooksTable::updateBook(database* db, const Book& book) {
    std::string sql = "UPDATE books SET title = ?, author = ?, genre = ?, isbn = ? WHERE id = ?";
    sqlStatement* statement;
    if (!prepareStatement(db, sql, &statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        return false;
    }
    setText(statement, 1, book.mTitle);
    setText(statement, 2, book.mAuthor);
    setText(statement, 3, book.mGenre);
    setText(statement, 4, book.mIsbn);
    setInt(statement, 5, book.mId);
    if (!evaluateStatement(statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        sqlFinalize(statement);
        return false;
    }
    const bool found = sqlite3_changes(db) > 0;
    sqlFinalize(statement);
    if (!found)
        std::cerr << "Book not found." << std::endl;
    return found;
}

bool BooksTable::deleteBook(database* db, int bookId) {
    std::string sql = "DELETE FROM books WHERE id = ?";
    sqlStatement* statement;
    if (!prepareStatement(db, sql, &statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        return false;
    }
    setInt(statement, 1, bookId);
    if (!evaluateStatement(statement)) {
        std::cerr << "SQL error: " << errorMessage(db) << std::endl;
        sqlFinalize(statement);
        return false;
    }
    const bool found = sqlite3_changes(db) > 0;
    sqlFinalize(statement);
    if (!found)
        std::cerr << "Book not found." << std::endl;
    return found;
}