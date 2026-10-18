#ifndef SQLITEWRAPPER_HPP
#define SQLITEWRAPPER_HPP

#include <sqlite3.h>
#include <string>
#include <stdint.h>

typedef sqlite3_stmt sqlStatement;
typedef sqlite3 database;

inline bool openDatabase(const std::string& dbName, database** db) {
    return (sqlite3_open(dbName.c_str(), db) == SQLITE_OK);
}

inline bool closeDatabase(database* db) {
    return (sqlite3_close(db) == SQLITE_OK);
}

inline bool prepareStatement(database* db, std::string str, sqlStatement** stmt){
    return (sqlite3_prepare_v2(db, str.c_str(), -1, stmt, 0) == SQLITE_OK);
}

inline void setText(sqlStatement* stmt, uint8_t position, std::string str){
    sqlite3_bind_text(stmt, position, str.c_str(), -1, SQLITE_TRANSIENT);
}

inline void setInt(sqlStatement* stmt, uint8_t position, int value) {
    sqlite3_bind_int(stmt, position, value);
}

inline bool evaluateStatement(sqlStatement* stmt){
    return (sqlite3_step(stmt) == SQLITE_DONE);
}

inline bool hasRow(sqlStatement* stmt) {
    return (sqlite3_step(stmt) == SQLITE_ROW);
}

inline const char* errorMessage(database* db){
    return sqlite3_errmsg(db);
}

inline uint16_t sqlFinalize(sqlStatement* stmt){
    return sqlite3_finalize(stmt);
}

inline std::string getText(sqlStatement* stmt, uint8_t column) {
    return std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, column)));
}

inline uint8_t getInt(sqlStatement* stmt, uint8_t column) {
    return sqlite3_column_int(stmt, column);
}

bool prepAndEvaluate(database* db, std::string str);

#endif // SQLITEWRAPPER_HPP