// parser.cpp
#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

// Helper function to split input into tokens
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void Parser::parse(const std::string& input, std::unordered_map<std::string, Database>& databases, std::string& currentDatabase) {
    std::istringstream inputStream(input);
    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(inputStream, line)) {
        std::vector<std::string> lineTokens = split(line, ' ');
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }

    if (tokens.empty()) {
        return;
    }

    std::string command = tokens[0];
    if (command == "CREATE" && tokens[1] == "DATABASE") {
        std::string dbName = tokens[2];
        databases[dbName] = Database();
        std::cout << "Database " << dbName << " created." << std::endl;
    } else if (command == "USE" && tokens[1] == "DATABASE") {
        std::string dbName = tokens[2];
        if (databases.find(dbName) != databases.end()) {
            currentDatabase = dbName;
            databases[dbName].load(dbName + ".db");
            std::cout << "Using database " << dbName << "." << std::endl;
        } else {
            std::cerr << "Database " << dbName << " does not exist." << std::endl;
        }
    } else if (currentDatabase.empty()) {
        std::cerr << "No database selected." << std::endl;
    } else {
        Database& db = databases[currentDatabase];
        if (command == "CREATE" && tokens[1] == "TABLE") {
            std::string tableName = tokens[2];
            Table table(tableName);
            size_t i = 4; // Skip "CREATE TABLE tableName ("
            while (i < tokens.size() && tokens[i] != ")") {
                std::string columnName = tokens[i];
                std::string columnType = tokens[i + 1];
                table.addColumn(columnName, columnType);
                i += 3; // Skip "columnName columnType ,"
            }
            db.createTable(tableName);
        } else if (command == "DROP") {
            std::string table;
            inputStream >> table;
            if (table == "TABLE") {
                std::string tableName;
                inputStream >> tableName;
                db.dropTable(tableName);
            }
        } else if (command == "SELECT") {
            std::string tableName;
            inputStream >> tableName;
            Table* table = db.getTable(tableName);
            if (table) {
                table->queryTable();
            } else {
                std::cout << "Table " << tableName << " does not exist." << std::endl;
            }
        } else if (command == "EXIT") {
            std::cout << "See ya next time!" << std::endl;
            exit(0);
        } else if (command == "HELP") {
            std::cout << "Commands available: CREATE TABLE, DROP TABLE, SELECT, EXIT" << std::endl;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
}