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
            for (auto& subToken : subTokens) {
                if (!subToken.empty() && subToken != " " && subToken != ",") {
                    tokens.push_back(subToken);
                }
            }
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
            db.createTable(tableName, table);
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
            std::vector<std::string> columns;
            bool selectAll = false;
            size_t i = 1;
            if (tokens[i] == "*") {
                selectAll = true;
                i++;
            } else {
                while (i < tokens.size() && tokens[i] != "FROM") {
                    if (tokens[i] != ",") {
                        columns.push_back(tokens[i]);
                    }
                    i++;
                }
            }
            if (tokens[i] == "FROM") {
                tableName = tokens[++i];
                Table* table = db.getTable(tableName);
                if (table) {
                    if (selectAll) {
                        if (i + 2 < tokens.size() && tokens[i + 1] == "WHERE") {
                            std::string whereColumn = tokens[i + 2];
                            std::string whereOperator = tokens[i + 3];
                            std::string whereValueStr = tokens[i + 4];
                            if (whereValueStr.back() == ';') {
                                whereValueStr.pop_back(); // Remove ending semicolon
                            }
                            ColumnType whereValue;
                            if (whereValueStr.front() == '"') {
                                whereValue = whereValueStr.substr(1, whereValueStr.size() - 2); // Remove quotes
                            } else if (whereValueStr.find('.') != std::string::npos) {
                                whereValue = std::stod(whereValueStr);
                            } else {
                                whereValue = std::stoi(whereValueStr);
                            }
                            table->queryTable(columns, whereColumn, whereOperator, whereValue);
                        } else {
                            table->queryTable();
                        }
                    } else {
                        std::cout<<"i + 2 < tokens.size()"<<(i + 2 < tokens.size())<<std::endl;
                        std::cout<<"tokens[i + 1] == WHERE"<<(tokens[i + 1] == "WHERE")<<std::endl;
                        if (i + 2 < tokens.size() && tokens[i + 1] == "WHERE") {
                            std::string whereColumn = tokens[i + 2];
                            std::cout<<"whereColumn: "<<whereColumn<<std::endl;
                            std::string whereOperator = tokens[i + 3];
                            std::cout<<"whereOperator: "<<whereOperator<<std::endl;
                            std::string whereValueStr = tokens[i + 4];
                            std::cout<<"whereValueStr: "<<whereValueStr<<std::endl;

                            if (whereValueStr.back() == ';') {
                                whereValueStr.pop_back(); // Remove ending semicolon
                            }
                            ColumnType whereValue;
                            if (whereValueStr.front() == '"') {
                                whereValue = whereValueStr.substr(1, whereValueStr.size() - 2); // Remove quotes
                            } else if (whereValueStr.find('.') != std::string::npos) {
                                whereValue = std::stod(whereValueStr);
                            } else {
                                whereValue = std::stoi(whereValueStr);
                            }
                            table->queryTable(columns, whereColumn, whereOperator, whereValue);
                        } else {
                            table->queryTable(columns);
                        }
                    }
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid SELECT syntax." << std::endl;
            }
        } else if (command == "INSERT" && tokens[1] == "INTO") {
            std::string tableName = tokens[2];
            if (tokens[3] == "VALUES" && tokens[4] == "(" && tokens.back() == ")") {
                std::cout<<"Inserting into table: "<<tableName<<std::endl;
                Table* table = db.getTable(tableName);
                if (table) {
                    std::vector<ColumnType> values;
                    for (size_t i = 5; i < tokens.size() - 1; ++i) {
                        if (tokens[i] == ",") continue; // Ignore commas
                        ColumnType value;
                        if (tokens[i].front() == '"' ) {
                            std::string strValue = tokens[i].substr(1); // Remove starting quote
                            while (i < tokens.size() - 1 && tokens[i].back() != '"') {
                                strValue += " " + tokens[++i];
                            }
                            if (tokens[i].back() == '"') {
                                strValue.pop_back(); // Remove ending quote
                            }
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
                        values.push_back(value);
                    }
                    table->insertRow(values);
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid INSERT INTO syntax." << std::endl;
            }
        } else if (command == "UPDATE") {
            std::string tableName = tokens[1];
            if (tokens[2] == "SET") {
                std::string columnName = tokens[3];
                std::string operation = tokens[4];
                std::string valueStr = tokens[5];
                ColumnType value;
                if (valueStr.front() == '"') {
                    value = valueStr.substr(1, valueStr.size() - 2); // Remove quotes
                } else if (valueStr.find('.') != std::string::npos) {
                    value = std::stod(valueStr);
                } else {
                    value = std::stoi(valueStr);
                }

                Table* table = db.getTable(tableName);
                if (table) {
                    if (tokens.size() > 6 && tokens[6] == "WHERE") {
                        std::string whereColumn = tokens[7];
                        std::string whereOperator = tokens[8];
                        std::string whereValueStr = tokens[9];
                        if (whereValueStr.back() == ';') {
                            whereValueStr.pop_back(); // Remove ending semicolon
                        }
                        ColumnType whereValue;
                        if (whereValueStr.front() == '"') {
                            whereValue = whereValueStr.substr(1, whereValueStr.size() - 2); // Remove quotes
                        } else if (whereValueStr.find('.') != std::string::npos) {
                            whereValue = std::stod(whereValueStr);
                        } else {
                            whereValue = std::stoi(whereValueStr);
                        }
                        table->updateColumn(columnName, operation, value, whereColumn, whereOperator, whereValue);
                    } else {
                        table->updateColumn(columnName, operation, value);
                    }
                    std::cout << "Table " << tableName << " updated." << std::endl;
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid UPDATE syntax." << std::endl;
            }
        } else if (command == "DELETE") {
            if (tokens[1] == "FROM") {
                std::string tableName = tokens[2];
                Table* table = db.getTable(tableName);
                if (table) {
                    if (tokens.size() > 3 && tokens[3] == "WHERE") {
                        std::string whereColumn = tokens[4];
                        std::string whereOperator = tokens[5];
                        std::string whereValueStr = tokens[6];
                        if (whereValueStr.back() == ';') {
                            whereValueStr.pop_back(); // Remove ending semicolon
                        }
                        ColumnType whereValue;
                        if (whereValueStr.front() == '"') {
                            whereValue = whereValueStr.substr(1, whereValueStr.size() - 2); // Remove quotes
                        } else if (whereValueStr.find('.') != std::string::npos) {
                            whereValue = std::stod(whereValueStr);
                        } else {
                            whereValue = std::stoi(whereValueStr);
                        }
                        table->deleteRows(whereColumn, whereOperator, whereValue);
                    } else {
                        std::cerr << "Invalid DELETE syntax. Missing WHERE clause." << std::endl;
                    }
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid DELETE syntax." << std::endl;
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