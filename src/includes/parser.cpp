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
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void Parser::parse(const std::string& input, std::unordered_map<std::string, Database>& databases, std::string& currentDatabase) {
    std::istringstream inputStream(input);
    std::string line;
    std::vector<std::string> tokens;

    while (std::getline(inputStream, line)) {
        std::vector<std::string> lineTokens = split(line, ' ');
        for (auto& token : lineTokens) {
            std::vector<std::string> subTokens = split(token, ';');
            tokens.insert(tokens.end(), subTokens.begin(), subTokens.end());
        }
    }

    if (tokens.empty()) {
        std::cout<<"Please input the correct command"<<std::endl;
        return;
    }

    std::string command = tokens[0];
    if (command == "CREATE" && tokens[1] == "DATABASE") {
        std::string dbName = tokens[2];
        databases[dbName] = Database();
        // 创建 dbName.db 文件
        std::ofstream dbFile(dbName + ".db");
        if (dbFile) {
            dbFile.close();
            std::cout << "Database " << dbName << " created." << std::endl;
        } else {
            std::cerr << "Failed to create database file: " << dbName << ".db" << std::endl;
        }
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
                i += 2; 
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
        } else if (command == "INSERT" && tokens[1] == "INTO") {
            std::string tableName = tokens[2];
            if (tokens[3] == "VALUES" && tokens[4] == "(" && tokens.back() == ")") {
                Table* table = db.getTable(tableName);
                if (table) {
                    std::unordered_map<ColumnType, ColumnType> row;
                    for (size_t i = 5; i < tokens.size() - 1; ++i) {
                        if (tokens[i] == ",") continue; // Ignore commas
                        ColumnType value;
                        if (tokens[i].front() == '"' ) {
                            std::string strValue = tokens[i].substr(1); // Remove starting quote
                            while (i < tokens.size() - 1 && tokens[i].back() != '"') {
                                strValue += " " + tokens[++i];
                            }
                            strValue.pop_back(); // Remove ending quote
                            value = strValue;
                        } else {
                            try {
                                if (tokens[i].find('.') != std::string::npos) {
                                    value = std::stod(tokens[i]);
                                } else {
                                    value = std::stoi(tokens[i]);
                                }
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Invalid value: " << tokens[i] << std::endl;
                                return;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Value out of range: " << tokens[i] << std::endl;
                                return;
                            }
                        }
                        row[tokens[i - 1]] = value; // Use the correct key type
                    }
                    table->insertRow(row);
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid INSERT INTO syntax." << std::endl;
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