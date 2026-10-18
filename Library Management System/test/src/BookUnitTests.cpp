#include <gtest/gtest.h>
#include "Book.hpp"
#include "Table.hpp"
#include "SQLiteWrapper.hpp"
#include <algorithm>
#include <memory>

class BookTest : public ::testing::Test {
protected:
    database* db;
    uint64_t isbn = 0;
    std::vector<Book> books;
    
    void SetUp() override {
        openDatabase("TestDatabase", &db);
        BooksTable::createBooksTable(db);
    }
    
    void TearDown() override {
        closeDatabase(db);
    }
};

TEST_F(BookTest, AddBook) {
    //Assert
    ASSERT_TRUE(BooksTable::addBook(db, Book(1, "Test Title1", "Test Author1", "Test Genre1", "111111")));

    //Clean
    ASSERT_TRUE(BooksTable::clearBooksTable(db));
}

TEST_F(BookTest, ViewBooks) {
    //Arrange
    const uint16_t numberOfBooks = 4;
    Book book1(0, "Test Title1", "Test Author1", "Test Genre1", "111111");
    Book book2(0, "Test Title2", "Test Author2", "Test Genre2", "222222");
    Book book3(0, "Test Title3", "Test Author4", "Test Genre3", "333333");
    Book book4(0, "Test Title4", "Test Author4", "Test Genre4", "444444");
    BooksTable::addBook(db, book1);
    BooksTable::addBook(db, book2);
    BooksTable::addBook(db, book3);
    BooksTable::addBook(db, book4);
    std::array<Book,numberOfBooks> testBooks = {book1,book2,book3,book4};

    //Act
    books = BooksTable::viewBooks(db);

    //Assert
    for(int i=0;i<books.size();i++)
    {   
        ASSERT_EQ(books[i],testBooks[i]);
    }

    //Clean
    BooksTable::clearBooksTable(db);
}

TEST_F(BookTest, UpdateBook) {
    //Arrange
    Book book(0, "Test Title", "Test Author", "Test Genre", "111111");
    Book bookTest(0, "Updated Title", "Test Author", "Updated Genre", "111111");
    BooksTable::addBook(db, book);
    books = BooksTable::viewBooks(db);

    //Act
    book.mId = books[0].mId;
    book.mTitle = "Updated Title";
    book.mGenre = "Updated Genre";
    ASSERT_TRUE(BooksTable::updateBook(db, book));
    books = BooksTable::viewBooks(db);

    //ASSERT
    ASSERT_EQ(books[0],bookTest);

    //Clean
    BooksTable::clearBooksTable(db);
}

TEST_F(BookTest, DeleteBook) {
    //Arrange
    Book book = {0, "Test Title", "Test Author", "Test Genre", "111111"};
    BooksTable::addBook(db, book);
    
    //Act
    books = BooksTable::viewBooks(db);
    ASSERT_TRUE(BooksTable::deleteBook(db, books[0].mId));

    //Assert
    books = BooksTable::viewBooks(db);
    ASSERT_TRUE(books.size() == 0);

    //Clean
    BooksTable::clearBooksTable(db);
}

TEST(SQLiteBindingTest, BookTextSurvivesBindingUntilStatementExecution) {
    database* rawDb = nullptr;
    const bool opened = openDatabase(":memory:", &rawDb);
    std::unique_ptr<database, decltype(&sqlite3_close)> db(rawDb, sqlite3_close);
    ASSERT_TRUE(opened);
    ASSERT_TRUE(BooksTable::createBooksTable(db.get()));

    const Book shortBook(0, "Title", "Author", "Genre", "9781234567890");
    const Book longBook(0, std::string(128, 'T'), std::string(128, 'A'),
                       std::string(128, 'G'), "9780987654321");
    ASSERT_TRUE(BooksTable::addBook(db.get(), shortBook));
    ASSERT_TRUE(BooksTable::addBook(db.get(), longBook));

    const auto stored = BooksTable::viewBooks(db.get());
    ASSERT_EQ(stored.size(), 2u);
    EXPECT_NE(std::find(stored.begin(), stored.end(), shortBook), stored.end());
    EXPECT_NE(std::find(stored.begin(), stored.end(), longBook), stored.end());
}

TEST(BookIdTest, LargeIdsSurviveReadUpdateAndDelete) {
    database* rawDb = nullptr;
    const bool opened = openDatabase(":memory:", &rawDb);
    std::unique_ptr<database, decltype(&sqlite3_close)> db(rawDb, sqlite3_close);
    ASSERT_TRUE(opened);
    ASSERT_TRUE(BooksTable::createBooksTable(db.get()));

    for (const int id : {255, 256, 65536}) {
        SCOPED_TRACE(id);
        ASSERT_TRUE(prepAndEvaluate(db.get(),
            "INSERT INTO books (id, title, author, genre, isbn) VALUES (" +
            std::to_string(id) + ", 'Original', 'Author', 'Genre', '9781234567890')"));
        auto books = BooksTable::viewBooks(db.get());
        ASSERT_EQ(books.size(), 1u);
        ASSERT_EQ(books[0].mId, id);

        books[0].mTitle = "Updated";
        ASSERT_TRUE(BooksTable::updateBook(db.get(), books[0]));
        books = BooksTable::viewBooks(db.get());
        ASSERT_EQ(books.size(), 1u);
        EXPECT_EQ(books[0].mId, id);
        EXPECT_EQ(books[0].mTitle, "Updated");

        ASSERT_TRUE(BooksTable::deleteBook(db.get(), books[0].mId));
        EXPECT_TRUE(BooksTable::viewBooks(db.get()).empty());
    }
}

TEST(BookInsertTest, DuplicateIsbnPreservesOriginalBook) {
    database* rawDb = nullptr;
    const bool opened = openDatabase(":memory:", &rawDb);
    std::unique_ptr<database, decltype(&sqlite3_close)> db(rawDb, sqlite3_close);
    ASSERT_TRUE(opened);
    ASSERT_TRUE(BooksTable::createBooksTable(db.get()));

    const Book original(0, "Original", "Original Author", "Fiction", "9781234567890");
    ASSERT_TRUE(BooksTable::addBook(db.get(), original));
    const auto before = BooksTable::viewBooks(db.get());
    ASSERT_EQ(before.size(), 1u);

    const Book duplicate(0, "Replacement", "Different Author", "History", original.mIsbn);
    EXPECT_FALSE(BooksTable::addBook(db.get(), duplicate));
    const auto after = BooksTable::viewBooks(db.get());
    ASSERT_EQ(after.size(), 1u);
    EXPECT_EQ(after[0].mId, before[0].mId);
    EXPECT_EQ(after[0], original);

    ASSERT_TRUE(BooksTable::addBook(db.get(), Book(0, "Another", "Author", "Genre", "9780987654321")));
    EXPECT_EQ(BooksTable::viewBooks(db.get()).size(), 2u);
}

TEST(BookMutationTest, MissingBookIdsDoNotReportSuccess) {
    database* rawDb = nullptr;
    const bool opened = openDatabase(":memory:", &rawDb);
    std::unique_ptr<database, decltype(&sqlite3_close)> db(rawDb, sqlite3_close);
    ASSERT_TRUE(opened);
    ASSERT_TRUE(BooksTable::createBooksTable(db.get()));
    ASSERT_TRUE(BooksTable::addBook(db.get(), Book(0, "Original", "Author", "Genre", "9781234567890")));
    auto books = BooksTable::viewBooks(db.get());
    ASSERT_EQ(books.size(), 1u);
    const Book original = books[0];
    Book missing = original;
    missing.mId++;
    missing.mTitle = "Missing";

    EXPECT_FALSE(BooksTable::updateBook(db.get(), missing));
    EXPECT_FALSE(BooksTable::deleteBook(db.get(), missing.mId));
    books = BooksTable::viewBooks(db.get());
    ASSERT_EQ(books.size(), 1u);
    EXPECT_EQ(books[0].mId, original.mId);
    EXPECT_EQ(books[0], original);

    EXPECT_TRUE(BooksTable::updateBook(db.get(), original));
    ASSERT_TRUE(BooksTable::deleteBook(db.get(), original.mId));
    EXPECT_FALSE(BooksTable::deleteBook(db.get(), original.mId));
    EXPECT_FALSE(BooksTable::updateBook(db.get(), original));
    EXPECT_TRUE(BooksTable::viewBooks(db.get()).empty());
}
